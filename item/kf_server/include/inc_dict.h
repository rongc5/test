#ifndef __INC_DICT_HEADER__
#define __INC_DICT_HEADER__

#include <sys/types.h>
#include <algorithm>
#include <new>
#include "link_pool.h"
#include "im_vector.h"
#include "log_helper.h"

template <typename Type>
struct dict_node_t{
	uint32_t first_sign;
	uint32_t second_sign;
	Type obj;
};

template <typename Type> class inc_dict_t
{
	private:
		typedef struct
		{
			MP_POINT new_list_head;
		}hash_entry_t;

		typedef struct
		{
			int hash_num;
			hash_entry_t * pp_hash;
		}inc_hash_t;

	public:
		typedef struct{
			uint32_t tra_level_idx;
			MP_POINT cur_node_addr;
		}travel_info_t;

	private:
		typedef link_node_t< dict_node_t<Type> > hash_node_t;
		
		static const int DFT_HASH_SIZE		= 5000011;
		static const int HALF_SIGN_WIDTH	= 32;
		static const int LOW_SIGN_MASK		= 0xFFFFFFFF;
		static const int m_prime_list[];
		static const int DFT_DIRTY_SIZE = 100003;
		
	public:
		inc_dict_t();
		~inc_dict_t();

	public:
		//create
		int create(int hash_size, int chunk_size, link_node_pool_t< dict_node_t<Type> > * p_pool = NULL, 
			uint32_t dirty_size = DFT_DIRTY_SIZE);

		//traverse
		Type * get_begin(travel_info_t * p_tra_info, uint32_t sign_list[2]);
		Type * get_next(travel_info_t * p_tra_info, uint32_t sign_list[2]);
		Type * get_end(travel_info_t * p_tra_info);

		//seek
		Type * seek_node(const uint32_t sign_list[2], MP_POINT * ptr = NULL);

		//modify
		Type * add_node(const uint32_t sign_list[2], 
			const Type * p_data, MP_POINT node = MP_NULL_POINT);
		int remove_node(const uint32_t sign_list[2]);
		int remove_node(const MP_POINT node)
		{
			Type * real_node = m_p_node_pool->get_real_point(node);
			if (real_node != NULL)
			{
				uint32_t sign[2] = {real_node->sign1, real_node->sign2};
				return remove_node(sign);
			}
			else
			{
				return -1;
			}
		}
		
		//clean all info
		int renew();

		//recycle dirty nodes
		int recycle();

		int remove_all_if(bool (* cmp_func)(const Type *));

		//get memory used
		size_t get_mem_consume();
		
		//get node num
		uint32_t get_node_num();
		
		//test use
		int tranverse_print();
/**
***监控接口
**/
		void get_detect_info(char * buf, uint32_t len)
		{
			snprintf(buf, len, "mem_consume:%lu\tnode_num:%u\thash_size:%u\t", 
				m_mem_consume, m_node_num, m_hash.hash_num);
		}
		
	private:
		int get_prime_num(int num);
		int build_table(int hash_num);

	private:
		size_t m_mem_consume;
		uint32_t m_node_num;
		inc_hash_t m_hash;

		/* memory pool */
		bool m_is_private_pool;
		link_node_pool_t< dict_node_t<Type> > * m_p_node_pool;
		im_vector_t <MP_POINT> * m_p_dirty_vector;
	public:
		MP_POINT get_node()
		{
			return m_p_node_pool->get_link_node();
		}
		int get_real_address(MP_POINT node, uint32_t * & key, Type * & value)
		{
			hash_node_t * real_node = m_p_node_pool->get_real_point(node);
			if (NULL == real_node)
			{
				key = NULL;
				value = NULL;

				return -1;
			}
			else
			{
				key = (uint32_t *)&(real_node->cu_data.first_sign);
				value = (Type *) &(real_node->cu_data.obj);
				return 0;
			}
		}
		
};

template <typename Type>
const int inc_dict_t<Type>::m_prime_list[] = {
	1009,   2003,   3001,   4001,   5003,   6007,   7001,   8009,   9001,
	10007,  20011,  30011,  40009,  50021,  60013,  70001,  80021,  90001,
	100003, 200003, 300007, 400009, 500009, 600011, 700001, 800011, 900001,
	1000003,2000003,3000017,4000037,5000011,6000011,7000003,8000009,9000011,
	12582917,   25165843,   50331653,   100663319,
	201326611,  402653189,  805306457,  1610612741
};

template <typename Type>
int inc_dict_t<Type>::get_prime_num(int num)
{
	const int prime_num = sizeof(m_prime_list)/sizeof(int);
	int turn = 0;
	for(turn = 0; turn < prime_num; ++turn){
		if(m_prime_list[turn] >= num){
			break;
		}
	}

	if(turn >= prime_num){
		return m_prime_list[prime_num - 1];
	}else{
		return m_prime_list[turn];
	}
}


template <typename Type> inc_dict_t<Type>::inc_dict_t()
	: m_mem_consume(sizeof(inc_dict_t<Type>))
	, m_node_num(0)
	, m_is_private_pool(false)
	, m_p_node_pool(NULL)
	, m_p_dirty_vector(NULL)
{
	memset(&m_hash, 0, sizeof(inc_hash_t));
}

template <typename Type> inc_dict_t<Type>::~inc_dict_t()
{
	free(m_hash.pp_hash);
	if(m_is_private_pool){
		delete m_p_node_pool;
	}
}

template <typename Type>
int inc_dict_t<Type>::create(int hash_size, int chunk_size, link_node_pool_t< dict_node_t<Type> > * p_pool, 
	uint32_t dirty_size)
{
	if(0 > build_table(hash_size)){
		return -1;
	}

	if(NULL != p_pool){
		m_p_node_pool = p_pool;
	}else{
		try{
			m_p_node_pool = new link_node_pool_t< dict_node_t<Type> >(chunk_size);
		}
		catch(std::bad_alloc){
			LOG_WARNING("Failed to allocate memory for inc_dict");
			return -1;
		}
		m_is_private_pool = true;
	}

	if (dirty_size > (uint32_t)hash_size/10)
	{
		dirty_size = (uint32_t)hash_size/10;
	}
	m_p_dirty_vector = new (std::nothrow) im_vector_t<MP_POINT>(0xffffffff);
	if (NULL == m_p_dirty_vector)
	{
		return -1;
	}
	return m_p_dirty_vector->create(dirty_size);
}

template <typename Type> 
int inc_dict_t<Type>::build_table(int hash_num)
{
	const int new_hash_num = get_prime_num(hash_num);
	if((0 < m_hash.hash_num) && (NULL != m_hash.pp_hash)){
		if(new_hash_num <= m_hash.hash_num){
			return 0;
		}
	}

	hash_entry_t * pp_new_hash = (hash_entry_t *)realloc(m_hash.pp_hash, new_hash_num * sizeof(hash_entry_t));
	if(NULL == pp_new_hash){
		LOG_WARNING("Failed to allocate memory for hash table");
		return -1;
	}else{
		m_mem_consume += (new_hash_num - m_hash.hash_num) * sizeof(hash_entry_t);
		m_hash.hash_num = new_hash_num;
		m_hash.pp_hash = pp_new_hash;
	}
	memset(m_hash.pp_hash, 0, m_hash.hash_num * sizeof(hash_entry_t));

	return 0;	
}


template <typename Type>
Type * inc_dict_t<Type>::get_begin(travel_info_t * p_tra_info, uint32_t sign_list[2])
{
	int hash_num = m_hash.hash_num;
	hash_entry_t * pp_hash =  m_hash.pp_hash;
	
	if((NULL == p_tra_info) || (0 >= hash_num) || (NULL == pp_hash))
	{
		return NULL;
	}
	
	int tra_level_idx = 0;
	MP_POINT mp_tra_node = MP_NULL_POINT;
	hash_node_t * real_node = NULL;
	
	for(tra_level_idx = 0; tra_level_idx < hash_num; ++tra_level_idx){
		if(MP_NULL_POINT == pp_hash[tra_level_idx].new_list_head){
			continue;
		}
		
		mp_tra_node = pp_hash[tra_level_idx].new_list_head;
		real_node = m_p_node_pool->get_real_point(mp_tra_node);
		if(real_node == NULL)
		{
			continue;
		}
		
		p_tra_info->tra_level_idx = tra_level_idx;
		p_tra_info->cur_node_addr = real_node->p_next;
		sign_list[0] = real_node->cu_data.first_sign;
		sign_list[1] = real_node->cu_data.second_sign;
		
		return &(real_node->cu_data.obj);
	}

	return NULL;
}

template <typename Type>
Type * inc_dict_t<Type>::get_next(travel_info_t * p_tra_info, uint32_t sign_list[2])
{
	if(NULL == p_tra_info)
	{
		return NULL;
	}
	
	hash_node_t * real_node = m_p_node_pool->get_real_point(p_tra_info->cur_node_addr);
	if(real_node != NULL)
	{
		p_tra_info->cur_node_addr = real_node->p_next;
		sign_list[0] = real_node->cu_data.first_sign;
		sign_list[1] = real_node->cu_data.second_sign;
		return &(real_node->cu_data.obj);
	}
				
	int hash_num = m_hash.hash_num;
	hash_entry_t * pp_hash =  m_hash.pp_hash;
	int tra_level_idx = p_tra_info->tra_level_idx;
	MP_POINT mp_tra_node = MP_NULL_POINT;
	
	tra_level_idx ++;
	for(; tra_level_idx < hash_num; ++tra_level_idx){
		if(MP_NULL_POINT == pp_hash[tra_level_idx].new_list_head){
			continue;
		}
		
		mp_tra_node = pp_hash[tra_level_idx].new_list_head;
		real_node = m_p_node_pool->get_real_point(mp_tra_node);
		if(real_node == NULL)
		{
			continue;				
		}
			
		p_tra_info->tra_level_idx = tra_level_idx;
		p_tra_info->cur_node_addr = real_node->p_next;
		sign_list[0] = real_node->cu_data.first_sign;
		sign_list[1] = real_node->cu_data.second_sign;
		
		return &(real_node->cu_data.obj);
	}

	return NULL;
}

template <typename Type>
Type * inc_dict_t<Type>::get_end(travel_info_t * p_tra_info)
{
	return NULL;
}


template <typename Type>
Type * inc_dict_t<Type>::seek_node(const uint32_t sign_list[2], MP_POINT * ptr)
{
	if((0 >= m_hash.hash_num) || (NULL == m_hash.pp_hash)){
		return NULL;
	}

	unsigned long key_sign = ((unsigned long)sign_list[0]) << HALF_SIGN_WIDTH | sign_list[1];
	int key_pos = key_sign % m_hash.hash_num;
	MP_POINT mp_cur_node = m_hash.pp_hash[key_pos].new_list_head;
	
	hash_node_t * real_node = NULL;
	unsigned long data_sign = 0;
	
	while(MP_NULL_POINT != mp_cur_node)
	{
		real_node = m_p_node_pool->get_real_point(mp_cur_node);
		data_sign = (((unsigned long)real_node->cu_data.first_sign) << HALF_SIGN_WIDTH) |
					real_node->cu_data.second_sign;		
		if(key_sign == data_sign){
			if (NULL != ptr)
			{
				*ptr = mp_cur_node;
			}
			
			return &(real_node->cu_data.obj);
		}
		mp_cur_node = real_node->p_next;
	}

	return NULL;
}

template <typename Type>
Type * inc_dict_t<Type>::add_node(const uint32_t sign_list[2], const Type * p_data, MP_POINT mp_node)
{
	if(NULL == p_data){
		return NULL;
	}

	if (MP_NULL_POINT == mp_node)
	{
		mp_node = m_p_node_pool->get_link_node();
		if(MP_NULL_POINT == mp_node)
		{
			LOG_WARNING("Failed to get node from link pool");
			return NULL;
		}
	}

	hash_node_t * real_node = m_p_node_pool->get_real_point(mp_node);
	real_node->cu_data.first_sign = sign_list[0];
	real_node->cu_data.second_sign = sign_list[1];
	real_node->cu_data.obj = *p_data;

	/**
	 * 注意: link_pool 不对分配的空间置零, 需要在这里对所有需要置初值的域赋值!
	 *
	**/
//	real_node->cu_data.link = MP_NULL_POINT;

	real_node->p_next = MP_NULL_POINT;

	unsigned long key_sign = (((unsigned long)sign_list[0]) << HALF_SIGN_WIDTH) | sign_list[1];
	int key_pos = key_sign % m_hash.hash_num;
	hash_entry_t * cur_entry = m_hash.pp_hash + key_pos;
	
	real_node->p_next = cur_entry->new_list_head;
	cur_entry->new_list_head = mp_node;

	m_node_num ++;
	
	return &(real_node->cu_data.obj);
}

template <typename Type>
int inc_dict_t<Type>::remove_node(const uint32_t sign_list[2])
{
	if((0 >= m_hash.hash_num) || (NULL == m_hash.pp_hash)){
		return -1;
	}

	unsigned long key_sign = ((unsigned long)sign_list[0]) << HALF_SIGN_WIDTH | sign_list[1];
	int key_pos = key_sign % m_hash.hash_num;
	MP_POINT mp_cur_node = m_hash.pp_hash[key_pos].new_list_head;
	MP_POINT mp_pre_node = mp_cur_node;
	hash_node_t * real_node = NULL;
	unsigned long data_sign = 0;
	
	while(MP_NULL_POINT != mp_cur_node){
		real_node = m_p_node_pool->get_real_point(mp_cur_node);
		data_sign = (((unsigned long)real_node->cu_data.first_sign) << HALF_SIGN_WIDTH) |
					real_node->cu_data.second_sign;		
		if(key_sign == data_sign){
			break;
		}
		mp_pre_node = mp_cur_node;
		mp_cur_node = real_node->p_next;
	}

	if(MP_NULL_POINT != mp_cur_node)
	{
		if(m_hash.pp_hash[key_pos].new_list_head == mp_cur_node)
		{
			m_hash.pp_hash[key_pos].new_list_head = real_node->p_next;
		}
		else
		{
			hash_node_t * pre_real_node = NULL;
		
			pre_real_node = m_p_node_pool->get_real_point(mp_pre_node);
			pre_real_node->p_next = real_node->p_next;
		}

		m_p_dirty_vector->push_back(mp_cur_node);
		
		m_node_num --;
	}

	return 0;
}

template <typename Type> 
int inc_dict_t<Type>::recycle()
{
	/* hash is empty */
	if((0 >= m_hash.hash_num) || (NULL == m_hash.pp_hash)){
		memset(&m_hash, 0, sizeof(inc_hash_t));
		return 0;
	}

	/* recycle all nodes from hash */
			
			
	for (uint32_t i = 0; i<m_p_dirty_vector->size(); i++)
	{
		m_p_node_pool->put_link_node((*m_p_dirty_vector)[i]);
	}
	
	m_p_dirty_vector->clear();
	
	if(m_is_private_pool)
	{
		m_p_node_pool->recycle();
	}
	return 0;
}

template <typename Type> int inc_dict_t<Type>::renew()
{
	if(!m_is_private_pool)
	{
		if((0 >= m_hash.hash_num) || (NULL == m_hash.pp_hash)){
			memset(&m_hash, 0, sizeof(inc_hash_t));
			return 0;
		}
	
		hash_entry_t * pp_hash = m_hash.pp_hash;
		MP_POINT mp_cur_node = MP_NULL_POINT;
		MP_POINT mp_next_node = MP_NULL_POINT;
	
		for(int hash_idx = 0; hash_idx < m_hash.hash_num; ++hash_idx){
			if(MP_NULL_POINT == pp_hash[hash_idx].new_list_head){
				continue;
			}
	
			mp_cur_node = pp_hash[hash_idx].new_list_head;
			mp_next_node = mp_cur_node;
			pp_hash[hash_idx].new_list_head = MP_NULL_POINT;
			while(MP_NULL_POINT != mp_cur_node){
				mp_next_node = m_p_node_pool->get_real_point(mp_cur_node)->p_next;
				m_p_node_pool->put_link_node(mp_cur_node);

				mp_cur_node = mp_next_node;
			}
		}
		
		recycle();
	}
	else
	{
		if(NULL != m_hash.pp_hash)
		{
			memset(m_hash.pp_hash, 0, m_hash.hash_num * sizeof(hash_entry_t));
		}

		m_p_node_pool->reset();
		m_p_dirty_vector->clear();
	}

	m_node_num = 0;
	
	return 0;
}

template <typename Type> size_t inc_dict_t<Type>::get_mem_consume()
{
	size_t used_mem = m_mem_consume;
	if(m_is_private_pool){
		used_mem += m_p_node_pool->get_mem_consume();
	}
	return used_mem + sizeof(MP_POINT) * m_p_dirty_vector->size();
}


template <typename Type> uint32_t inc_dict_t<Type>::get_node_num()
{
	return m_node_num;
}

template <typename Type>
int inc_dict_t<Type>::tranverse_print()
{
	printf("\n----------tranverse_print----------\n\n");
	
	int hash_num = m_hash.hash_num;
	hash_entry_t * pp_hash =  m_hash.pp_hash;
	int level_idx = 0;
	MP_POINT mp_cur_node = MP_NULL_POINT;
	hash_node_t * real_node = NULL;
	
	for(level_idx = 0; level_idx < hash_num; ++level_idx){
		if((MP_NULL_POINT == pp_hash[level_idx].new_list_head)
			&& (MP_NULL_POINT == pp_hash[level_idx].old_list_head))
		{
			continue;
		}
		
		printf("entry:\t%d\n", level_idx);
		
		//new list
		mp_cur_node = pp_hash[level_idx].new_list_head;
		if(mp_cur_node != MP_NULL_POINT)
		{
			printf("new_list_head\t");
		}
		
		while(MP_NULL_POINT != mp_cur_node)
		{
			real_node = m_p_node_pool->get_real_point(mp_cur_node);
			if(real_node == NULL)
			{
				break;
			}
			
			printf("%u,%u-->", real_node->cu_data.first_sign, real_node->cu_data.second_sign);
			mp_cur_node = real_node->p_next;
		}
		
		printf("\n");
		
		//old list
		mp_cur_node = pp_hash[level_idx].old_list_head;
		if(mp_cur_node != MP_NULL_POINT)
		{
			printf("old_list_head\t");
		}
		
		hash_node_t * cur_head_node = NULL;
		while(MP_NULL_POINT != mp_cur_node)
		{
			real_node = m_p_node_pool->get_real_point(mp_cur_node);
			if(real_node == NULL)
			{
				break;
			}
			
			printf("%u,%u-->", real_node->cu_data.first_sign, real_node->cu_data.second_sign);
			if(mp_cur_node == pp_hash[level_idx].old_list_tail)
			{
				cur_head_node = m_p_node_pool->get_real_point(real_node->p_next);
				if(cur_head_node == NULL)
				{
					printf("|||| NULL\n");
				}
				else
				{
					printf("||||%u,%u\n", cur_head_node->cu_data.first_sign, cur_head_node->cu_data.second_sign);
				}
				
				break;
			}
			else
			{
				mp_cur_node = real_node->p_next;
			}
		}
		
		printf("------------------------------\n");
	}
	
	return 0;
}

#endif

// vim: ts=4 sts=4 sw=4 noet

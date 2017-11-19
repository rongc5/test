#ifndef LINK_POOL_H_INCLUDED
#define LINK_POOL_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "common_util.h"

namespace im {
namespace lib {

typedef uint32_t MP_POINT;

const MP_POINT MP_NULL_POINT = 0U;

template <typename Type> struct link_node_t {
	MP_POINT p_next;
	Type cu_data;
};      

template <typename Type> class link_node_pool_t {
	private:
		typedef link_node_pool_t<Type> self_t;
		typedef link_node_t<Type> node_t;

		static const uint32_t MP_POINT_WIDTH = sizeof(MP_POINT) * 8U - 4;
		static const uint32_t MIN_NODE_WIDTH = 10U;
		static const uint32_t MAX_NODE_WIDTH = 20U;
		static const uint32_t INIT_BLOCK_NUM = 1024U;
		static const uint32_t mp_block_size_list[];

	public:
		link_node_pool_t(uint32_t block_size = 0U);
		~link_node_pool_t();

	public:
		node_t *get_real_point(MP_POINT mp_point) {
			return const_cast<node_t*>(static_cast<const self_t&>(*this).get_real_point(mp_point));
		}

		const node_t *get_real_point(MP_POINT mp_point) const {
			if (mp_point == MP_NULL_POINT) {
				return NULL;
			}

			uint32_t block_idx = (mp_point >> m_node_width);
			uint32_t node_idx = (mp_point & m_node_mask);

			if (block_idx >= m_alloc_block_num || node_idx >= m_block_size) {
				PDEBUG("INVALID mp_point %u %u %u %u\n", 
					block_idx, m_alloc_block_num, node_idx, m_block_size);
				return NULL;
			}

			return m_block_array[block_idx] + node_idx;
		}

		MP_POINT get_link_node();

		int put_link_node(MP_POINT p_node);
		void recycle();
		void reset();

		size_t get_mem_consume() const {
			return m_mem_consume;
		}

		// 测试用接口...
		void display_free_list() const;
		void display_dirty_list() const;

	private:
		int add_block();
		void set_pool_para(uint32_t block_size);

	public:
		uint32_t m_node_width;
		uint32_t m_node_mask;
		uint32_t m_max_block_num;
		uint32_t m_block_size;

		uint32_t m_block_num;
		uint32_t m_alloc_block_num;
		uint32_t m_avail_num;
		uint32_t m_cur_block_idx;

		MP_POINT m_dirty_head;
		MP_POINT m_free_head;
		MP_POINT m_dirty_tail;
		MP_POINT m_free_tail;

		node_t **m_block_array;

		size_t m_mem_consume;

};

template <typename Type>
const uint32_t link_node_pool_t<Type>::mp_block_size_list[] = {
	1024U, 2048U, 3072U, 4096U, 5120U, 6144U, 7168U, 8192U, 9216U,
	10240U, 20480U, 30720U, 40960U, 51200U, 61440U, 71680U, 81920U, 92160U,
	102400U, 204800U, 307200U, 409600U, 512000U, 614400U, 716800U, 819200U, 921600U,
	1048576U
};

template <typename Type> 
link_node_pool_t<Type>::link_node_pool_t(uint32_t block_size)
	:
	m_node_width(0U),
	m_node_mask(0U),
	m_max_block_num(0U),
	m_block_size(0U),
	m_block_num(0U),
	m_alloc_block_num(0U),
	m_avail_num(0U),
	m_cur_block_idx(0U),
	m_dirty_head(MP_NULL_POINT),
	m_free_head(MP_NULL_POINT),
	m_dirty_tail(MP_NULL_POINT),
	m_free_tail(MP_NULL_POINT),
	m_block_array(NULL),
	m_mem_consume(sizeof(link_node_pool_t<Type>)) {
	set_pool_para(block_size);
}

template <typename Type>
link_node_pool_t<Type>::~link_node_pool_t() {
	if (m_block_array != NULL) {
		for (uint32_t i = 0; i < m_alloc_block_num; i++) {
			free(m_block_array[i]);
		}

		free(m_block_array);
	}
}

template <typename Type>
int link_node_pool_t<Type>::add_block() {
	if (m_cur_block_idx >= m_alloc_block_num) {
		if (m_block_array == NULL) {
			m_block_array = (node_t**)malloc(sizeof(node_t*) * INIT_BLOCK_NUM);

			if (m_block_array == NULL) {
			   PDEBUG("cannot mallocate memory for m_block_array");
				return -1;
			}

			m_mem_consume += sizeof(node_t*) * INIT_BLOCK_NUM;
			m_block_num = INIT_BLOCK_NUM;
		}

		if (m_alloc_block_num >= m_max_block_num) {
			PDEBUG("block_num has reach upper limit[%d], block_size is [%d]", m_max_block_num, m_block_size);
			return -1;
		}

		if (m_alloc_block_num >= m_block_num) {
			uint32_t new_block_num = m_block_num * 2U;
			node_t **p_new_block_array = (node_t**)realloc(m_block_array, sizeof(node_t*) * new_block_num);

			if (p_new_block_array == NULL) {
				PDEBUG("realloc memory for m_block_array fail");
				return -1;
			}

			m_mem_consume += sizeof(node_t*) * (new_block_num - m_block_num);
			m_block_num = new_block_num;
			m_block_array = p_new_block_array;
		}

		m_block_array[m_alloc_block_num] = (node_t*)malloc(sizeof(node_t) * m_block_size);

		if (m_block_array[m_alloc_block_num] == NULL) {
			PDEBUG("cannot mallocate memory for block");
			return -1;
		}

		m_mem_consume += sizeof(node_t) * m_block_size;
		m_alloc_block_num++;
	}

	m_cur_block_idx++;
	m_avail_num = m_block_size;

	return 0;
}

template <typename Type>
MP_POINT link_node_pool_t<Type>::get_link_node() {
	MP_POINT p_node;

	if (m_free_head != MP_NULL_POINT) {
		// 首先试图从 free 链中获取...
		p_node = m_free_head;

		m_free_head = get_real_point(m_free_head)->p_next;

		if (m_free_head == MP_NULL_POINT) {
			m_free_tail = MP_NULL_POINT;
		}

		get_real_point(p_node)->p_next = MP_NULL_POINT;
	}
	else if (m_avail_num == 0U && add_block() == -1) {
		// 检查当前块可用大小, 并在大小不足时添加新的块, 失败则返回 MP_NULL_POINT ...
		p_node = MP_NULL_POINT;
	}
	else {
		if (m_cur_block_idx == 1U && m_avail_num == m_block_size) {
			// 预留 MP_NULL_POINT (0U), 不能分配这个特殊地址...
			m_avail_num--;
		}

		if (m_cur_block_idx >= 1U) {
			// 如果曾有一个 add_block() 成功了, 那 m_cur_block_idx 就会大于 0 ...
			p_node = (((m_cur_block_idx - 1U) << m_node_width) | (m_block_size - m_avail_num));
			get_real_point(p_node)->p_next = MP_NULL_POINT;
			m_avail_num--;
		}
		else {
			p_node = MP_NULL_POINT;
		}
	}

	return p_node;
}

template <typename Type>
int link_node_pool_t<Type>::put_link_node(MP_POINT p_node) {
	if (p_node == MP_NULL_POINT) {
		return -1;
	}

	get_real_point(p_node)->p_next = MP_NULL_POINT;

	if (m_dirty_head == MP_NULL_POINT) {
		m_dirty_head = p_node;
		m_dirty_tail = p_node;
	}
	else {
		get_real_point(m_dirty_tail)->p_next = p_node;
		m_dirty_tail = p_node;
	}

	return 0;
}

template <typename Type>
void link_node_pool_t<Type>::set_pool_para(uint32_t block_size) {
	uint32_t size_num = sizeof(mp_block_size_list) / sizeof(uint32_t);
	uint32_t i;

	for (i = 0U; i < size_num; i++) {
		if (mp_block_size_list[i] >= block_size) {
			break;
		}
	}

	if (i >= size_num) {
		i = size_num - 1U;
	}

	m_block_size = mp_block_size_list[i];

	for (i = MIN_NODE_WIDTH; i <= MAX_NODE_WIDTH; i++) {
		if ((1U << i) >= m_block_size) {
			break;
		}
	}

	m_node_width = i;

	m_node_mask = (1U << m_node_width) - 1U;
	m_max_block_num = (1U << (MP_POINT_WIDTH - m_node_width));
}

template <typename Type>
void link_node_pool_t<Type>::recycle() {
	if (m_dirty_head != MP_NULL_POINT) {
		if (m_free_head == MP_NULL_POINT) {
			m_free_head = m_dirty_head;
			m_free_tail = m_dirty_tail;
		}
		else {
			get_real_point(m_free_tail)->p_next = m_dirty_head;
			m_free_tail = m_dirty_tail;
		}

		m_dirty_head = MP_NULL_POINT;
		m_dirty_tail = MP_NULL_POINT;
	}
}

template <typename Type>
void link_node_pool_t<Type>::reset() {
	if (m_alloc_block_num >= 1U) {
		m_avail_num = m_block_size;
		m_cur_block_idx = 1U;
	}
	else {
		// 此处需要确认 m_avail_num 为零, 否则下一次申请时不会判定为需要申请新块...
		m_alloc_block_num = 0U;
		m_avail_num = 0U;
		m_cur_block_idx = 0U;
	}

	m_dirty_head = MP_NULL_POINT;
	m_dirty_tail = MP_NULL_POINT;

	m_free_head = MP_NULL_POINT;
	m_free_tail = MP_NULL_POINT;
}

template <typename Type>
void link_node_pool_t<Type>::display_free_list() const {
	if (m_free_head == MP_NULL_POINT) {
		fputs("\n-------- free list null --------\n", stdout);
	}
	else {
		fputs("\n-------- free list --------\n", stdout);

		MP_POINT list_head = m_free_head;

		while (list_head != MP_NULL_POINT) {
			fprintf(stdout, "%u->", list_head);
			list_head = get_real_point(list_head)->p_next;
		}

		fputs("\n\n", stdout);
	}
}

template <typename Type>
void link_node_pool_t<Type>::display_dirty_list() const {
	if (m_dirty_head == MP_NULL_POINT) {
		fputs("\n-------- dirty list null --------\n", stdout);
	}
	else {
		fputs("\n-------- dirty list --------\n", stdout);

		MP_POINT list_head = m_dirty_head;

		while (list_head != MP_NULL_POINT) {
			fprintf(stdout, "%u->", list_head);
			list_head = get_real_point(list_head)->p_next;
		}

		fputs("\n\n", stdout);
	}
}

}; /* @ namespace lib */
}; /* @ namespace im */

using im::lib::MP_POINT;
using im::lib::MP_NULL_POINT;
using im::lib::link_node_t;
using im::lib::link_node_pool_t;

#endif /* ! LINK_POOL_H_INCLUDED */

// vim: ts=4 sts=4 sw=4 noet

#ifndef __DOUBLE_BUFFER_H_
#define __DOUBLE_BUFFER_H_


namespace MZFRAME{


template<class T>
class double_buffer{
	
	public:
		double_buffer():_curr(0){
				_objects_[0] = new T;
				_objects_[1] = new T;
			}
		
		ClassT* Current() {
        if( _curr == 0 || _curr == 1){
            return &(objects_[_curr]);
        }

        return NULL;
    }
		
		
		ClassT* Idle() {
        if( _curr == 0 || _curr == 1){
            return &(objects_[1 - _curr]);
        }

        return NULL;
    }
    
    void exchange() {	
    		_curr = 1 - _curr;
    }
		
		
		~double_buffer {
				if (_objects[0]){
					delete 	_objects_[0];
				}
				
				if (_objects_[1]) {
						delete 	_objects_[1];
				}
			}
		
	
	private:
		char _curr;
		T * _objects[2];
	};



}
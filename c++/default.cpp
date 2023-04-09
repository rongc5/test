#include <iostream>

class Magic {
    public:
        Magic() = default; // ��ʽ����ʹ�ñ��������ɵĹ���
        Magic& operator=(const Magic&) = delete; // ��ʽ�����ܾ����������ɹ���
        Magic(int magic_number){
            _value = magic_number;
        }

        int get_value()
        {
            return _value;
        }

    private:
        int _value;
};

int main(int argc, char *argv[])
{
    Magic m;
    std::cout << m.get_value() << "\n";
    Magic n(100);
    std::cout << n.get_value() << "\n";
    
    return 0;
}

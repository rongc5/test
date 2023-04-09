#include <iostream>
#include <tuple>



auto get_student(int id)
{
    if (id == 0)
        return std::make_tuple(3.8,'A');
    if (id == 1)
        return std::make_tuple(3.9,'B');
    if (id == 2)
        return std::make_tuple(4.0,'C');

    return std::make_tuple(0.0, 'D');
}


int main(int argc, char *argv[])
{
    auto student = get_student(0);

    std::cout << "ID: 0, "
        << "GPA: " << std::get<0>(student) << ", "
        << " 成绩: " << std::get<1>(student) << std::endl;

    double gpa;
    char grade;

    std::tie(gpa, grade) = get_student(1);
    std::cout << "ID: 1, "
        << "GPA: " << gpa << ", "
        << " 成绩: " << grade << std::endl;


    //for(int i = 0; i != tuple_len(student); ++i)
    //{
        //std::cout << tuple_index(student, i) << std::endl;
    //}
    
    return 0;
}

#include <iostream>
#include <sstream>


using namespace std;

class Student {
    private:
        int m_age;
        int m_id;
        int m_grade_1;
        int m_grade_2;
        int m_grade_3;
        int m_grade_4;
        string m_name;

    public:
        Student(int age, int id, int grade_1, int grade_2, int grade_3,
                int grade_4, const string &name)
          : m_age(age),
            m_id(id),
            m_grade_1(grade_1),
            m_grade_2(grade_2),
            m_grade_3(grade_3),
            m_grade_4(grade_4),
            m_name(name)
        {}

        string getInfo()
        {
            int average = (m_grade_1 + m_grade_2 + m_grade_3 + m_grade_4) >> 2;
            stringstream str_stream;
            str_stream << m_name << ',' << m_age << ',' << m_id << ',' << average;
            return str_stream.str();
        }
};


int main()
{
    int age, id, grade_1, grade_2, grade_3, grade_4;
    string str_blah;

    getline(cin, str_blah, ',');
    string name(str_blah);

    getline(cin, str_blah, ',');
    stringstream(str_blah) >> age;

    getline(cin, str_blah, ',');
    stringstream(str_blah) >> id;

    getline(cin, str_blah, ',');
    stringstream(str_blah) >> grade_1;

    getline(cin, str_blah, ',');
    stringstream(str_blah) >> grade_2;

    getline(cin, str_blah, ',');
    stringstream(str_blah) >> grade_3;

    getline(cin, str_blah);
    stringstream(str_blah) >> grade_4;

    Student student(age, id, grade_1, grade_2, grade_3, grade_4, name);
    string str_result = student.getInfo();
    cout << str_result << endl;

    return 0;
}


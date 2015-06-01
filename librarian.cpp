/**********************************************************
 * Author        : oyjb
 * Email         : jbouyang@126.com
 * Last modified : 2015-05-30 16:29
 * Filename      : class.cpp
 * Description   : This is a librarian addministration programming.
 * *******************************************************/
/*
 * 程序名称：图书管理系统
 * 图书编号（唯一关键字）、书名、是否在架
 * 维护：记录新增、梗概、删除、查找、显示、全部删除、图书记录保存在指定的文件中
 * 作者:oyjb
 */
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int maxborrow = 5; /*最多借阅5本图书*/
void manager();  /*图书管理函数*/


/*
 * 管理员类*/
/*##################################################################################
 */
class Librarian
{
public:
	Librarian();  //默认构造函数
	Librarian(int tag,int no,string name,string passwd) : del_tag(tag),librarian_no(no),librarian_name(name),librarian_passwd(passwd){}
	
	int get_del_tag(); //获得删除标记
	int get_librarian_no(); //获得管理编号
	string get_librarian_name(); //获得管理员姓名
	string get_librarian_passwd(); //获得管理员密码
	void set_librarian_no(int no); //设置管理员编号
	void set_librarian_name(string name); //设置管理员姓名
	void set_librarian_passwd(string passwd); //设置管理员密码
	void del_librarian(); //删除管理员
	void restore_librarian(); //恢复管理员权限（即删除标记置为0）

	bool check(string name,string passwd); //核对用户名和密码

private:
	int del_tag; /*删除标记   1.已删  0.未删*/
	int librarian_no;  //管理员编号
	string librarian_name; //管理员姓名
	string librarian_passwd; //管理员密码
};

Librarian::Librarian(){}

int Librarian::get_del_tag()
{
	return del_tag;
}

int Librarian::get_librarian_no()
{
	return librarian_no;
}

string Librarian::get_librarian_name()
{
	return librarian_name;
}

string Librarian::get_librarian_passwd()
{
	return librarian_passwd;
}

void Librarian::set_librarian_no(int no)
{
	librarian_no = no;
}

void Librarian::set_librarian_name(string name)
{
	librarian_name = name;
}

void Librarian::set_librarian_passwd(string passwd)
{	
	librarian_passwd = passwd;
}

void Librarian::del_librarian()
{
	del_tag = 1;
}

void Librarian::restore_librarian()
{
	del_tag = 0;
}

bool Librarian::check(string name,string passwd)
{
	return ( (name == librarian_name) && (passwd == librarian_passwd) );
}
/* 以上为管理员类部分*/


/*
 * ##################################################################################
 *管理员库类
 */
class LibDataBase
{
	/* 把管理员类设置为管理员库的友元
	 * 这样就可以使得每一个管理员都可以操作管理员库中的数据
	 * 比如向其中添加管理员
	 */
	friend class Librarian;
public:
	LibDataBase(); //构造函数
	~LibDataBase(); //析构函数
	void clear();  //清楚函数
	void add_librarian(int no,string name,string passwd); //添加管理员

	Librarian *query(int no); //根据管理编号查询
	Librarian *query(string name); //根据管理员姓名查询

	void librariandata(); //管理员维护函数

private:
	vector<Librarian> lib; //管理员库,声明列表，存放的值是管理员类型
};

LibDataBase::LibDataBase()
{
	Librarian l;
	string line,item;
	ifstream input("librarian.dat");
	while (getline(input,line))
	{
		/* 设置Librarian对象的管理员编号 */
		istringstream record(line);
		record >> item;
		int no = stoi(item); //把string对象转换为int型
		l.set_librarian_no(no);
		
		/*设置Librarian对象的管理员姓名 */
		record >> item;
		l.set_librarian_name(item);

		/* 设置Librarian对象的mima */
		record >> item;
		l.set_librarian_passwd(item);

		/* 把Librarian对象加入到lib管理员库中 */
		lib.push_back(l);
	}
}

LibDataBase::~LibDataBase(){}

void LibDataBase::clear()
{	
	lib.clear();
}

void LibDataBase::add_librarian(int no,string name,string passwd)
{
	Librarian l(0,no,name,passwd); //用带三个参数的构造函数初始化一个Librarian对象
	lib.push_back(l);
}

Librarian *LibDataBase::query(int no)
{
	//遍历lib管理员库的每个成员的librarian_no成员
	for (auto it = lib.begin(); it != lib.end(); ++it)
	{
		if (it->get_librarian_no() == no && it->get_del_tag() == 0)
			return &(*it);
	}
	return NULL; //为找到匹配，返回空
}

Librarian *LibDataBase::query(string name)
{
	//遍历lib管理员库中的每个成员的librarian_name成员
	for (auto it = lib.begin(); it != lib.end(); ++it)
	{
		if (it->get_librarian_name() == name && it->get_del_tag() == 0)
			return &(*it);
	}
	return NULL;
}

void LibDataBase::librariandata()
{
	int libno; //管理员编号
	string libname; //管理员名字
	string libpasswd; //管理员密码
	int choice = 1; //选择操作
	int n;
	
	cout << "    " << endl;
	cout << "    " << endl;
	cout << "    " << endl;
	
	Librarian *l;
L0:	while(true)
	{
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                   ★★★★★管理员权限管理★★★★★                 │\n";
		cout<<"   ├───────┬────────┬────────┬─────────┤\n";
		cout<<"   │1:管理员登录  │  2:管理员注册  │3:管理员权限修改 │ 4:退出系统\n";
		cout<<"   ├───────┼────────┼────────┼─────────┤\n";
		cout<<"   │ 请您选择:                                                            │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin >> choice;
		switch(choice)
		{
			//管理员登陆
			case 1 :
			{
				int count = 0;
L1:				cout << "请输入管理员姓名: ";
				cin >> libname;
				l = query(libname);
				if (l == NULL)
				{
					++count; //输入次数加1
					if (count > 3)
					{
						cout << "您输入次数超过3次，您以被锁定！" << endl;
						exit(1);
					}
					cout << "对不起，用户名不存在，请重新输入!" << endl;
					goto L1;
				}
				else
				{
					count = 0; //重置count为0
L2:					cout << "请输入密码: ";
					cin >> libpasswd;
					if (!l->check(libname,libpasswd))
					{
						//密码错误
						++count;
						if (count > 3)
							exit(1);
						cout << "对不起，您输入的密码有误，请重新输入!" << endl;
						goto L2;
					}
					else
					{
						//用户名、密码均正确
						cout << "恭喜，您已成功登陆！" << endl;
						break;
					}
				}
				break;
			} //case 1
			


			//注册管理员
			case 2 :
			{
L3:				cout << "请输入您要注册的管理员编号: ";
				cin >> libno;
				//查询管理员库中是否已经存在该编号的管理员
				l = query(libno);
				if (l != NULL)
				{
					//已经存在该编号的管理员
					cout << "已经存在该编号的管理员，请重新输入要注册的管理员编号！" << endl;
					goto L3;
				}
				cout << "请输入您要注册的管理员姓名: ";
				cin >> libname;

				cout << "请设置管理员密码: ";
				cin >> libpasswd;
				
				//添加一个新的管理员
				add_librarian(libno,libname,libpasswd);
				cout << endl << endl;

				cout << "管理员注册成功！请牢记好以下信息: " << endl;
				cout << "######################################################################" << endl;
				cout << "管理员编号: " << libno << endl;
				cout << "管理员姓名: " << libname << endl;
				cout << "管理员密码: " << libpasswd << endl;
				cout << "######################################################################" << endl;

				break;
			} //case 2
			

			//管理员权限修改
			case 3 :
			{
				int choose;
				while (1)
				{
					cout << "\n\n\t1、管理员名称修改\t2、管理员密码修改\t3、返回上一级目录" << endl;
					cout << "\n\t请选择: ";
					cin >> choose;
					switch(choose)
					{
						//管理员姓名修改
						case 1 :
						{
L4:							cout << "请输入管理员编号: ";
							cin >> libno;
							l = query(libno);
							if (l == NULL)
							{
								cout << "管理员编号不存在，请重新输入！" << endl;
								goto L4;
							}
L5:							cout << "请输入管理员名称: ";
							cin >> libname;
							if (l->get_librarian_name() != libname)
							{
L6:								cout << "您输入的管理员编号与管理员姓名不匹配，请重新输入！" << endl;
								cout << "\n\t1、重新回到输入管理员编号  |  2、只重新输入管理员名称\n\n";
								int r;
								cin >> r;
								switch(r)
								{
									case 1 :
										goto L4;
										break;

									case 2 :
										goto L5;
										break;

									default:
										goto L6;
								}
							}
							
L7:							cout << "请输入密码: ";
							cin >> libpasswd;
							if (!l->check(libname,libpasswd))
							{
								cout << "您输入的密码有误，请重新输入！" << endl;
								goto L7;
							}
							cout << "请输入要设置的新的管理员名称: ";
							cin >> libname;
							l->set_librarian_name(libname);
							break;
						}
						
						//管理员密码
						case 2 :
						{
L8:							cout << "请输入管理员编号: ";
							cin >> libno;
							l = query(libno);
							if (l == NULL)
							{
								cout << "管理员编号不存在，请重新输入！" << endl;
								goto L8;
							}
L9:							cout << "请输入管理员名称: ";
							cin >> libname;
							if (l->get_librarian_name() != libname)
							{
L10:							cout << "您输入的管理员编号与管理员姓名不匹配，请重新输入！" << endl;
								cout << "\n\t1、重新回到输入管理员编号  |  2、只重新输入管理员名称\n\n";
								int r;
								cin >> r;
								switch(r)
								{
									case 1 :
										goto L8;
										break;

									case 2 :
										goto L9;
										break;

									default:
										goto L10;
								}
							}
L11:						cout << "请输入密码: ";
							cin >> libpasswd;
							if (!l->check(libname,libpasswd))
							{
								cout << "您输入的密码有误，请重新输入！" << endl;
								goto L11;
							}
							cout << "请输入要设置的新的密码: ";
							cin >> libpasswd;
							l->set_librarian_passwd(libpasswd);
							cout << "密码修改成功！" << endl;
							break; 
						}
						
						case 3 :
						{
							goto L0;
							break;
						}
						default:
							goto L0;
					}
				}
			break;
			} //case 3
		}
	}
}



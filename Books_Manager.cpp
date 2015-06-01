/**********************
程序名称:Books_Manager
程序功能:设计一个图书类，包含有图书编号（惟一关键字）、书名、在架否等私有数据成员。并实现图书维护操作，包括图书记录的新增、更改、删除（按
图书编号删除）、查找、显示和全部删除。并要求图书记录保存指定的文件中。
程序作者:oyjb
**********************/
#include <iostream>												//头文件
#include <list>
#include <iomanip>
#include <fstream>
#include <conio.h>
const int Maxborrow=5;											//最多借五本书
using namespace std;											//指定命名空间
void manager();													//图书管理函数
class Librarian													//图书管理员类
{
	int del_tag;												//删除标记	1.已删 0.未删
	int lib_No;													//管理员编号
	char lib_Name[50];											//管理员姓名
	char lib_Password[20];
public:
	Librarian();												//无参构造函数
	Librarian(int libNo,char libName[50],char libPassword[20]);	//带有三个参数的构造函数
	int getDelTag();											//获得删除标记函数
	int getLibNo();												//获得管理员编号函数
	char *getLibName();											//获得管理员名称函数
	char *getLibPassword();										//获得管理员密码函数
	void setLibrarianName(char libName[]);						//设置管理员名称函数
	void setLibrarianPassword(char libPassword[]);				//设置管理员密码函数
	void delLibrarian();										//删除管理员函数
	void addLibrarian(int libNo,char libName[],char libPassword[]);//添加管理员函数
	int strcmp1(char libName[]);								//核对用户名函数
	int strcmp2(char libPassword[]);							//核对用户名密码函数
};
Librarian::Librarian()
{}
Librarian::Librarian(int libNo,char libName[50],char libPassword[20])
{
	addLibrarian(libNo,libName,libPassword);					//调用添加管理员函数
}
int Librarian::getDelTag()
{
	return del_tag;												//返回删除标记
}
int Librarian::getLibNo()
{
	return lib_No;												//返回管理员编号
}
void Librarian::delLibrarian()
{
	del_tag=1;													//设置删除标记的值为1
}
void Librarian::addLibrarian(int libNo,char libName[],char libPassword[])
{
	del_tag=0;													//设置删除标记的值为0
	lib_No=libNo;												//给私有数据成员赋值
	strcpy(lib_Name,libName);
	strcpy(lib_Password,libPassword);
}
void Librarian::setLibrarianName(char libName[])				//实现设置姓名的函数
{
	strcpy(lib_Name,libName);					
}
void Librarian::setLibrarianPassword(char libPassword[])		//实现设置密码的函数
{
	strcpy(lib_Password,libPassword);
}
char *Librarian::getLibName()									//获得管理员姓名
{
	return lib_Name;
}
char *Librarian::getLibPassword()								//获得管理员密码
{
	return lib_Password;
}

int Librarian::strcmp1(char libName[])
{
	if(strcmp(libName,lib_Name)==0)								//核对用户输入的名称
		return 1;												//若匹配则返回值为1
	return 0;													//否则返回值为0
}
int Librarian::strcmp2(char libPassword[])						
{
	if(strcmp(libPassword,lib_Password)==0)						//核对用户输入的密码
		return 1;												//若匹配则返回值为1
	return 0;													//否则返回值为0
}

class LibDataBase												//管理员库类
{
	list<Librarian> lib;										//声明列表,存放的值是管理员类型
public:
	LibDataBase();												//构造函数
	~LibDataBase();												//析构函数
	void clear();												//清除函数
	int addLibrarian(int libNo,char libName[],char libPassword[]);//添加管理员函数
	Librarian *query(int libNo);								//根据管理员编号查询函数
	Librarian *query1(char libName[]);							//根据管理员名称查询函数
	Librarian *query2(char libPassword[]);						//根据管理员密码查询函数
	void librariandata();										//管理员维护函数
}
LibDataBase::LibDataBase()										//通过构造函数读文件
{
	Librarian l;												//创建一个管理员类的对象
	fstream file("librarian.dat",ios::in);						//用只读的方式打开文件
	while(1)													//当文件正常打开
	{
		file.read((char *)&l,sizeof(l));						//从文件中将数据读入内存
		if(!file)
			break;
		lib.push_back(l);										//将数据追加到列表的尾部
	}
	file.close();												//关闭文件
}
LibDataBase::~LibDataBase()										//通过析构函数写文件
{
	fstream file("librarian.dat",ios::out|ios::binary);			//如果文件存在则清除原文件,以只写方式打开
	list<Librarian>::iterator iter;								//通过列表创建一个迭代器
	for(iter=lib.begin();iter!=lib.end();iter++)				//从列表开始处得到一个迭代器,循环条件是迭代没到列表的末尾
	{
		if(iter->getDelTag()==0)								//如果删队标记的值为0,也就是未删除
			file.write((char *)&(*iter),sizeof(Librarian));		//将数据写入文件
	}
	file.close();												//关闭文件
	
}
void LibDataBase::clear()									
{
	lib.clear();												//清除整个列表
}
int LibDataBase::addLibrarian(int libNo,char libName[],char libPassword[])
{
	Librarian *p=query(libNo);									//定义一个指向管理员类的指针通过管理员编号查询得到结果
	if(p==NULL)													//如果指针内容为空
	{
		Librarian l(libNo,libName,libPassword);					//创建管理员的一个对象并传值过去
		lib.push_back(l);										//将此对象添加到列表的末尾
		return 1;												//返回为1表示添加成功
	}
	return 0;
}
Librarian *LibDataBase::query(int libNo)						//通过管理员编号查询,返回值为一个管理员类对象
{
	list<Librarian>::iterator iter;								//创建迭代器
	for(iter=lib.begin();iter!=lib.end();iter++)				//循环
	{
		if(iter->getLibNo()==libNo&&iter->getDelTag()==0)		//如果已存在的用户编码与输入的编码一到并且删队标记为0
			return &(*iter);									//表示存在,返回一个迭代器
	}
	return NULL;												//否则表示没有匹配的值，返回空
}
Librarian *LibDataBase::query1(char libName[])					//通过输入的管理员名查询
{	
	list<Librarian>::iterator iter;
	for(iter=lib.begin();iter!=lib.end();iter++)
	{
		if(strcmp(iter->getLibName(),libName)==0&&iter->getDelTag()==0)
			return &(*iter);
	}
	return NULL;
}
Librarian *LibDataBase::query2(char libPassword[])				//通过输入的管理员密码查询
{
	list<Librarian>::iterator iter;
	for(iter=lib.begin();iter!=lib.end();iter++)
	{
		if(strcmp(iter->getLibPassword(),libPassword)==0&&iter->getDelTag()==0)
			return &(*iter);
	}
	return NULL;
}

void LibDataBase::librariandata()								//管理员维护函数
{
	int choice=1;												//定义变量
	int temp=1;
	char libname[50];
	char libpassword[20];
	int libno;
	int estimate=0;
	int n=0;
	int count;
	int k=1;
	int k2=1;
	char c;
	system("cls");												//清屏
	system("color 7f");											//设置背景色
	cout<<"    "<<endl;
    cout<<"    "<<endl;
    cout<<"    "<<endl;
	Librarian *l;												//创建一个指向管理员类的指针
	while(choice!=0)
	{
		cout<<""<<endl;
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                   ★★★★★管理员权限管理★★★★★                 │\n";
		cout<<"   ├───────┬────────┬────────┬─────────┤\n";
		cout<<"   │1:管理员登录  │  2:管理员注册  │3:管理员权限修改│ 0:返回上一级目录 │\n";
		cout<<"   ├───────┼────────┼────────┼─────────┤\n";
		cout<<"   │ 请您选择:                                                            │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin>>choice;
		switch(choice)
		{	
		case 1:													//登录
a:				
			cout<<"\n\n\t\t\t请输入管理员名称:";
			cin>>libname;
			l=query1(libname);									//根据输入的管理员名称查询
			if(l==NULL)											//如果查询的结果为空
			{
				system("cls");
				system("color 7f");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"\n\n\t对不起,你的用户名不正确或用户名不存在!请重新输入或先注册:"<<endl;
				while(k<3)										//当输入次数在三次之内执行下面的语句
				{	
					k++;
					goto a;										//跳到a处继续循环
				}
				cout<<"\n\n\n\t\t并且您已输错三次用户名,请查证后再行登录!!!";
				break;
			}
			else
			{			
b:
			cout<<"\n\n\t\t\t请输入管理员密码:";
			libpassword[20]=0;
			count=0;
			while((c=getch())!='\r')							//当输入的字符不是回车
			{
				libpassword[count++]=c;
				putchar('*');
			}
			libpassword[count]='\0';
			
			estimate=l->strcmp2(libpassword);					//核对密码
			if(estimate==0)
			{	
				system("cls");
				system("color 7f");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"\n\n\t\t\t您的密码不正确!请重新输入:"<<endl;
				while(k2<3)				
				{	
					k2++;
					goto b;		
				}
				cout<<"\n\n\n\t\t并且您已输错三次密码,请查证后再行登录!!!";
				break;
			}
			else
			{
				system("cls");
				system("color 7f");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"\n\n\t\t\t\t恭喜登录成功!"<<endl;
				manager();
				break;
			}		
			}
			break;
		case 2:													//注册
f:
			cout<<"\n\n\t\t\t请输入您要注册的管理员编号(请牢记管理员编号):";
			cin>>libno;
			l=query(libno);										//根据编号查询
			if(l!=NULL)
			{
				cout<<"\n\n\t\t\t提示:该管理员已存在,请重新注册"<<endl;
				goto f;											//跳到f处继续执行
			}
			cout<<"\n\n\t\t\t请输入您要注册的管理员姓名:";
			cin>>libname;
			cout<<"\n\n\t\t\t请输入您要注册的管理密码:";
			count=0;
			while((c=getch())!='\r')
			{
				libpassword[count++]=c;
				putchar('*');
			}
			libpassword[count]='\0';
			addLibrarian(libno,libname,libpassword);			//添加管理员函数
			
			system("cls");
			system("color 7f");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t管理员注册成功!!!"<<endl;
			break;
		case 3:													//修改管理员
			system("cls");
			system("color 7f");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			while(temp!=0)
			{
				cout<<"\n\n\t1.管理员名称修改\t2.管理员密码修改\t0.返回上一级目录\n";
				cout<<"\n\n\t请选择:";
				cin>>temp;
				switch(temp)
				{
				case 1:											//修改管理员名称
					
					cout<<"\n\n\t\t\t请输入管理员编号:";
					cin>>libno;
					
d:
					l=query(libno);
					if(l==NULL)
					{
						cout<<"\n\n\t\t\t提示:该管理员不存在,请重新输入:"<<endl;
						goto d;
					}
					cout<<"\n\n\t\t\t请输入管理员名称:";
					cin>>libname;
					l=query1(libname);
					if(l==NULL)
					{
						cout<<"\n\n\t\t\t提示:该管理员不存在"<<endl;
						goto d;
					}
					cout<<"\n\n\t\t\t请输入新的管理员名称:";
					cin>>libname;
					l->setLibrarianName(libname);
					system("cls");
					system("color 7f");
					cout<<"    "<<endl;
					cout<<"    "<<endl;
					cout<<"    "<<endl;	
					cout<<"\n\n\t\t\t管理员名称修改成功!!!"<<endl;	
					break;
					
				case 2:											//修改管理员密码
					cout<<"\n\n\t\t\t请输入管理员编号:";
					cin>>libno;
e:
					cout<<"\n\n\t\t\t请输入管理员密码:";
					count=0;
					
					while((c=getch())!='\r')
					{
						libpassword[count++]=c;
						putchar('*');
					}
					libpassword[count]='\0';
					
					l=query2(libpassword);						//根据密码查询
					if(l==NULL)
					{
						cout<<"\n\n\t\t\t您输入的密码有误,请重新输入:"<<endl;
						goto e;
					}
					cout<<"\n\n\t\t\t请输入新的管理员密码:";
					
					count=0;
					while((c=getch())!='\r')
					{
						libpassword[count++]=c;
						putchar('*');
					}
					libpassword[count]='\0';
					l->setLibrarianPassword(libpassword);		//设置管理员密码
					system("cls");
					system("color 7f");
					cout<<"    "<<endl;
					cout<<"    "<<endl;
					cout<<"    "<<endl;
					cout<<"\n\n\t\t\t管理员密码修改成功!!!"<<endl;
					break;
					
				default:
					if(temp==0)
					{
						system("cls");
						system("color 7f");
						cout<<"    "<<endl;
						cout<<"    "<<endl;
						cout<<"    "<<endl;
						break;
					}
					break;
					cout<<"\n\n\t\t\t输入错误!!!\n请重新选择:";
					break;
					
				}
			}
			break;
			
		default:
			if(choice==0)
			{
				system("cls");
				system("color 7f");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				break;
			}
			break;
			cout<<"\n\n\t\t\t输入错误!!!\n请重新选择:";
			break;
		}
	}		
}

class Books															//图书类
{
	int del_tag;													//删除标记	1.已删	0.未删
	int book_no;													//图书编号
	char book_name[50];												//书名
	int book_onshelf;												//是否在架	1.在架	0.已借
public:
	Books();														//构造函数
	Books(int bookid,char bookname[]);								//重载构造函数
	char *getBookName();											//获取书名
	int getBookNo();												//获取图书编号
	int getDelTag();												//获取删除标记
	int borrowbook();												//借书操作
	void retbook();													//还书操作
	void setname(char bookname[]);									//设置书名
	void delbook();													//删除图书
	void addbook(int bookid,char bookname[]);						//增加图书
	void bookdisplay();												//显示一本图书
};
Books::Books()														//构造函数
{}
Books::Books(int bookid,char bookname[])							//重载构造函数
{
	addbook(bookid,bookname);										//增加图书
}
char *Books::getBookName()
{
	return book_name;												//获得书名
}
int Books::getBookNo()
{
	return book_no;													//获得书编号
}			
int Books::getDelTag()
{
	return del_tag;													//返回删除标记
}
int Books::borrowbook()												//借书函数
{
	if(book_onshelf==1)												//如果变量值为1，表示在架
	{
		book_onshelf=0;												//改变值为0
		return 1;													//返回真
	}
	return 0;														//否则返回假
}
void Books::retbook()												//还书
{
	book_onshelf=1;													//变量值为1，表示在架
}
void Books::setname(char bookname[])
{
	strcpy(book_name,bookname);										//设置书名
}
void Books::delbook()					
{
	del_tag=1;														//设置删除标记为1
}
void Books::addbook(int bookid,char bookname[])						
{
	del_tag=0;														//删除标记设值为0
	book_no=bookid;												
	strcpy(book_name,bookname);
	book_onshelf=1;													//在架
}
void Books::bookdisplay()											//输出单本图书函数
{
	cout<<"\n\n\t\t\t";
	cout<<setw(6)<<book_no<<setw(12)<<book_name<<setw(10)<<(book_onshelf==1?"在架":"已借")<<endl;
}

class BooksDataBase													//图书库类
{	
	list<Books> book;												//列表，存放的值为图书类
public:
	BooksDataBase();												//构造函数
	~BooksDataBase();												//析构函数
	void clear();													//清除函数
	int addbook(int bookid,char bookname[]);						//添加图书函数
	Books *query(int bookid);										//根据书的编号查询函数
	void bookdata();												//图书维护函数
	void bookdispall();												//输出所有图书函数
};

BooksDataBase::BooksDataBase()										//通过构造函数将数据写入文件
{
	Books b;
	fstream file("book.dat",ios::in);
	while(1)
	{
		file.read((char *)&b,sizeof(b));
		if(!file)
			break;
		book.push_back(b);
	}
	file.close();
}
BooksDataBase::~BooksDataBase()										//通过析构函数将数据写入文件
{	
	fstream file("book.dat",ios::out|ios::trunc);
	list<Books>::iterator iter;
	for(iter=book.begin();iter!=book.end();iter++)
	{
		if(iter->getDelTag()==0)
			file.write((char *)&(*iter),sizeof(Books));
	}
	file.close();
}
void BooksDataBase::clear()											//清除所有书籍
{
	book.clear();
}
int BooksDataBase::addbook(int bookid,char bookname[])				//添加书籍函数
{
	int s=0;
	ifstream ifile("serchBook.txt");
	if(ifile)														//如果文件能正常打开
	{
		ifile>>s;													//将文件中的数据输入到内存中
		ifile.close();
	}
	else
	{
		s=0;
	}
	Books *p=query(bookid);
	if(p==NULL)
	{
		Books b(bookid,bookname);
		book.push_back(b);
		s++;														//管理员每添加一本图书,图书记录加1
		ofstream tfile("serchbook.txt");							
		tfile<<s;													//将数据输出到文件中
		tfile.close();												//关闭文件
		return 1;
	}
	return 0;
}
Books *BooksDataBase::query(int bookid)								//根据图书编号查询函数
{
	list<Books>::iterator iter;
	for(iter=book.begin();iter!=book.end();iter++)
	{
		if(iter->getBookNo()==bookid&&iter->getDelTag()==0)
			return &(*iter);
		
	}
	return NULL;
}
void BooksDataBase::bookdispall()									//输出所有书籍函数
{
	list<Books>::iterator iter;
	if(book.empty())												//如果列表中为空
	{
		cout<<"\n\n\t\t\t对不起,此时没有图书!!!"<<endl;
	}
	else
	{
		int s=0;													
		ifstream ifile("serchBook.txt");							//打开文件
		if(ifile)													//如果文件打开正常
		{	
			ifile>>s;												//将文件中的记录数输入到内存中
			ifile.close();
		}
		else
		{
			s=0;
		}
		cout<<"\n\n\t\t\t已有图书"<<s<<"册"<<endl;
		cout<<"\n\n\t\t\t图书编号\t书名\t状态"<<endl;
		for(iter=book.begin();iter!=book.end();iter++)
		{
			if(iter->getDelTag()==0)
				iter->bookdisplay();
		}
	}
}
void BooksDataBase::bookdata()										//图书维护函数
{
	int choice=1;
	int n=0;
	char bookname[40];
	int bookid;
	system("cls");													//清屏
	system("color 1a");
	cout<<"    "<<endl;
    cout<<"    "<<endl;
    cout<<"    "<<endl;
	Books *b;
	while(choice!=0)
	{
		cout<<""<<endl;
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                       ★★★★★图书维护★★★★★                   │\n";
		cout<<"   ├────┬────┬────┬───┬───┬───┬────────┤\n";
		cout<<"   │1:新增  │2:更改  │3:删除  │4:查找│5:显示│6:全删│0:返回上一级目录│\n";
		cout<<"   ├────┼────┼────┼───┼───┼───┼────────┤\n";
		cout<<"   │ 请您选择:                                                            │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin>>choice;
		switch(choice)
		{
		case 1:
a:
			cout<<"\n\n\t\t\t输入图书编号:";
			cin>>bookid;
			b=query(bookid);										//根据书的编号查询
			if(b!=NULL)
			{
				cout<<"\n\n\t\t\t提示:该编号图书已存在,请重新添加:"<<endl;
				goto a;
				break;
			}
			cout<<"\n\n\t\t\t输入图书书名:";
			cin>>bookname;
			addbook(bookid,bookname);								//添加书籍
			cout<<"\n\n\t\t\t图书添加成功!!!"<<endl;
			cout<<"\n\n\t\t\t是否继续添加?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto a;
			else
			{
				system("cls");
				system("color 1a");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				continue;
			}
			break;
		case 2:														//修改函数
b:
			cout<<"\n\n\t\t\t输入图书编号:";
			cin>>bookid;
			b=query(bookid);
			if(b==NULL)
			{
				cout<<"\n\n\t\t\t提示:该图书不存在"<<endl;
				break;
			}
			cout<<"\n\n\t\t\t请输入新的书名:";
			cin>>bookname;
			b->setname(bookname);
			cout<<"\n\n\t\t\t图书修改成功!!!"<<endl;
			cout<<"\n\n\t\t\t是否继续修改?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto b;
			else
			{
				system("cls");
				system("color 1a");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				continue;
			}
			break;
		case 3:														//删除函数
c:
			cout<<"\n\n\t\t\t输入图书编号:";
			cin>>bookid;
			b=query(bookid);
			if(b==NULL)
			{
				cout<<"\n\n\t\t\t提示:该图书不存在"<<endl;
				break;
			}
			b->delbook();
			system("cls");
			system("color 1a");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t图书删除成功!"<<endl;
			cout<<"\n\n\t\t\t是否继续删除?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto c;
			else
			{
				system("cls");
				system("color 1a");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				continue;
			}
			break;
		case 4:														//查找函数
d:
			cout<<"\n\n\t\t\t输入图书编号:";
			cin>>bookid;
			b=query(bookid);
			if(b==NULL)
			{
				cout<<"\n\n\t\t\t提示:该图书不存在"<<endl;
			}
			system("cls");
			system("color 1a");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t图书编号\t书名\t状态"<<endl;
			b->bookdisplay();
			
			cout<<"\n\n\t\t\t图书查找成功!"<<endl;
			cout<<"\n\n\t\t\t是否继续查找?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto d;
			else
				system("cls");
			system("color 1a");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			continue;
			break;
		case 5:														//显示所有书籍
			system("cls");
			system("color 1a");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			bookdispall();
			break;
		case 6:														//删除所有书籍
			clear();
			system("cls");
			system("color 1a");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t提示:记录已全部删除!!!"<<endl;
			break;
		default:
			if(choice==0)
			{
				system("cls");
				system("color 1a");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				break;
			}
			break;
			cout<<"\n\n\t\t\t输入错误!!!\n请重新选择:";
			break;
		}
		
		
	}
}

class Reader														//读者类
{
	int del_tag;													//删除标记	1.已删	2.未删
	int read_no;													//读者编号
	char read_name[50];												//读者名
	int borrow_book[Maxborrow];										//所借图书
public:
	Reader();														//构造函数
	Reader(int readid,char readname[]);								//重载构造函数
	char *getReadName();											//获取读者名
	int getReadNo();												//获取读者编号
	int getDelTag();												//获取删除标记
	int retbook(int bookid);										//还书操作
	void borrowbook(int bookid);									//借书操作
	void setname(char readname[]);									//设置读者名
	void delread();													//删除图书读者
	void addread(int readid,char readname[]);						//增加读者
	void readdisplay();												//显示一本读者
};
Reader::Reader()													//构造函数
{}
Reader::Reader(int readid,char readname[])							//重载构造函数
{
	addread(readid,readname);
}
char *Reader::getReadName()
{
	return read_name;												//获得读者姓名
}
int Reader::getReadNo()
{
	return read_no;
}
int Reader::getDelTag()												//获得删除标记
{
	return del_tag;
}
int Reader::retbook(int bookid)										//还书函数
{
	for(int i=0;i<Maxborrow;i++)									//循环条件是i值小于最大借书数
	{
		if(borrow_book[i]==bookid)
		{
			borrow_book[i]=0;
			return 1;
		}
	}
	return 0;
}
void Reader::borrowbook(int bookid)									//借书函数
{
	for(int i=0;i<Maxborrow;i++)
	{
		if(borrow_book[i]=0)
		{
			borrow_book[i]=bookid;
			return;
		}
	}
}
void Reader::setname(char readname[])
{
	strcpy(read_name,readname);										//设置读者姓名
}
void Reader::delread()
{
	del_tag=1;														//设置删除标记的值为1
}
void Reader::addread(int readid,char readname[])					//添加读者函数
{
	del_tag=0;
	read_no=readid;
	strcpy(read_name,readname);
	for(int i=0;i<Maxborrow;i++)
		borrow_book[i]=0;
}
void Reader::readdisplay()											//输出单个读者函数
{
	cout<<"\n\n\t\t\t";
	cout<<setw(6)<<read_no<<setw(15)<<read_name<<endl;
}

class ReaderDataBase												//读者库类
{
	list<Reader> read;												//创建列表，存放的值为读者
public:
	ReaderDataBase();												//构造函数
	~ReaderDataBase();												//析构函数
	void clear();													//清除函数
	int addread(int readid,char readname[]);						//添加读者函数
	Reader *query(int readid);										//根据读者编号查询函数
	void readdata();												//读者维护函数
	void readdispall();												//输出所有读者函数
};

ReaderDataBase::ReaderDataBase()									//构造函数
{
	Reader r;														//创建读者类对象
	fstream file("read.dat",ios::in);
	while(1)
	{
		file.read((char *)&r,sizeof(r));
		if(!file)
			break;
		read.push_back(r);
	}
	file.close();
}
ReaderDataBase::~ReaderDataBase()									//通过析构函数将数据写入文件
{
	fstream file("read.dat",ios::out|ios::trunc);
	list<Reader>::iterator iter;
	for(iter=read.begin();iter!=read.end();iter++)
	{
		if(iter->getDelTag()==0)
			file.write((char *)&(*iter),sizeof(Reader));
	}
	file.close();
}
void ReaderDataBase::clear()										//清除函数
{
	read.clear();
}
int ReaderDataBase::addread(int readid,char readname[])				//添加读者函数
{
	Reader *p=query(readid);
	if(p==NULL)
	{
		Reader r(readid,readname);
		read.push_back(r);
		return 1;
	}
	return 0;
}
Reader *ReaderDataBase::query(int readid)							//查询函数
{
	list<Reader>::iterator iter;
	for(iter=read.begin();iter!=read.end();iter++)
	{
		if(iter->getReadNo()==readid&&iter->getDelTag()==0)
			return &(*iter);
		
	}
	return NULL;
}
void ReaderDataBase::readdispall()									//输出所有读者
{
	list<Reader>::iterator iter;
	if(read.empty())
	{
		cout<<"\n\n\t\t\t对不起,此时没有读者!!!"<<endl;
	}
	else
	{
		cout<<"\n\n\t\t\t读者编号\t读者姓名"<<endl;
		for(iter=read.begin();iter!=read.end();iter++)
		{
			if(iter->getDelTag()==0)
				iter->readdisplay();
		}
	}
}
void ReaderDataBase::readdata()										//读者维护
{
	int choice=1;
	int n=0;
	char readname[40];
	int readid;
	system("cls");
	system("color 5b");
	cout<<"    "<<endl;
    cout<<"    "<<endl;
    cout<<"    "<<endl;
	Reader *r;
	while(choice!=0)
	{
		cout<<""<<endl;
		cout<<""<<endl;
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                       ★★★★★读者维护★★★★★                   │\n";
		cout<<"   ├────┬────┬────┬───┬───┬───┬────────┤\n";
		cout<<"   │1:新增  │2:更改  │3:删除  │4:查找│5:显示│6:全删│0:返回上一级目录│\n";
		cout<<"   ├────┼────┼────┼───┼───┼───┼────────┤\n";
		cout<<"   │ 请您选择:                                                            │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin>>choice;
		switch(choice)
		{
		case 1:														//增加读者
a:
			cout<<"\n\n\t\t\t输入读者编号:";
			cin>>readid;
			r=query(readid);
			if(r!=NULL)
			{
				system("cls");
				system("color 5b");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"\n\n\t\t\t提示:该读者已存在"<<endl;
				goto a;
				break;
			}
			cout<<"\n\n\t\t\t输入读者书名:";
			cin>>readname;
			addread(readid,readname);
			cout<<"\n\n\t\t\t读者添加成功!!!"<<endl;
			cout<<"\n\n\t\t\t是否继续添加?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto a;
			else
				continue;
			break;
		case 2:														//修改读者
b:
			cout<<"\n\n\t\t\t输入读者编号:";
			cin>>readid;
			r=query(readid);
			if(r==NULL)
			{
				cout<<"\n\n\t\t\t提示:该读者不存在"<<endl;
				break;
			}
			cout<<"\n\n\t\t\t请输入新的书名:";
			cin>>readname;
			r->setname(readname);
			system("cls");
			system("color 5b");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t读者修改成功!!!"<<endl;
			cout<<"\n\n\t\t\t是否继续修改?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto b;
			else
				continue;
			break;
		case 3:														//删除读者
c:
			cout<<"\n\n\t\t\t输入读者编号:";
			cin>>readid;
			r=query(readid);
			if(r==NULL)
			{
				cout<<"\n\n\t\t\t提示:该读者不存在"<<endl;
				break;
			}
			r->delread();
			system("cls");
			system("color 5b");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"\n\n\t\t\t读者删除成功!"<<endl;
			cout<<"\n\n\t\t\t是否继续删除?\t1.是 2.否 3.按其他任意键返回上一级目录"<<endl;
			cin>>n;
			if(n==1)
				goto c;
			else
				continue;
			break;
		case 4:														//查询读者
			cout<<"\n\n\t\t\t输入读者编号:";
			cin>>readid;
			r=query(readid);
			if(r==NULL)
			{
				cout<<"\n\n\t\t\t提示:该读者不存在"<<endl;
				break;
			}
			system("cls");
			system("color 5b");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			r->readdisplay();
			break;
		case 5:														//显示所有读者
			system("cls");
			system("color 5b");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			readdispall();
			break;
		case 6:														//删除所有读者
			system("cls");
			system("color 5b");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			clear();
			cout<<"\n\n\t\t\t提示:记录已全部删除!!!"<<endl;
			break;
		default:
			if(choice==0)
			{
				system("cls");
				system("color 5b");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				
				break;
			}
			break;
			cout<<"\n\n\t\t\t输入错误!!!\n请重新选择:";
			break;
		}			
	}
}
void manager()														//图书管理函数
{
	
	int choice=1;
	int bookid,readerid;
	system("cls");
	system("color 7c");
	cout<<"    "<<endl;
    cout<<"    "<<endl;
    cout<<"    "<<endl;
	BooksDataBase bookDB;											//图书库类创建对象
	Books *book;													//图书类创建一个指针对象
	ReaderDataBase readerDB;										//读者库类创建对象
	Reader *read;													//读者类创建一个指针对象
	while(choice!=0)
	{
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                      ★★★★★图书管理★★★★★                    │\n";
		cout<<"   ├──────┬──────┬─────┬─────┬─────────┤\n";
		cout<<"   │1:图书维护  │  2:读者维护│3:借书    │ 4.还书   │ 0:返回上一级目录 │\n";
		cout<<"   ├──────┼──────┼─────┼─────┼─────────┤\n";
		cout<<"   │ 请您选择:                                                            │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin>>choice;
		switch(choice)
		{
		case 1:														//图书维护
			system("cls");
			system("color 7c");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			bookDB.bookdata();										//调用图书维护函数
			break;
		case 2:
			system("cls");
			system("color 7c");
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			cout<<"    "<<endl;
			readerDB.readdata();									//调用读者维护函数
			break;	
		case 3:														//借书
			cout<<"\n\n\t\t\t借书\n\n\t\t\t读者编号:";
			cin>>readerid;
			cout<<"\n\n\t\t\t图书编号:";
			cin>>bookid;
			read=readerDB.query(readerid);
			if(read==NULL)
			{
				cout<<"\n\n\t\t\t不存在该读者,不能借书!"<<endl;
				break;
			}
			book=bookDB.query(bookid);
			if(book==NULL)
			{
				cout<<"\n\n\t\t\t不存在该图书,不能借书!"<<endl;
				break;
			}
			if (book->borrowbook()==0)
			{
				cout<<"\n\n\t\t\t对不起,该图书已借出!"<<endl;
				break;
			}
			read->borrowbook(book->getBookNo());
			cout<<"\n\n\t\t\t借书成功!!!"<<endl;
			break;
		case 4:														//还书
			cout<<"\n\n\t\t\t还书\n\n\t\t\t读者编号:";
			cin>>readerid;
			cout<<"\n\n\t\t\t图书编号:";
			cin>>bookid;
			read=readerDB.query(readerid);
			if(read==NULL)
			{
				cout<<"\n\n\t\t\t不存在该读者,不能还书"<<endl;
				break;
			}
			book=bookDB.query(bookid);
			if(book==NULL)
			{
				cout<<"\n\n\t\t\t不存在该图书,不能还书"<<endl;
				break;
			}
			book->retbook();
			read->retbook(book->getBookNo());
			cout<<"\n\n\t\t\t还书成功!!!"<<endl;
			break;
		default:
			if(choice==0)
			{
				system("cls");
				system("color 7c");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				
				break;
			}
			break;
			cout<<"\n\n\t\t\t输入错误!!!\n请重新选择:";
			break;
		}
	}
}
void login()														//登录函数
{
	LibDataBase libDB;												//图书管理员类库创建对象
	libDB.librariandata();											//调用图书管理员维护函数
}
void main()															//主函数
{
	int h;
	while(1)
	{
		system("cls");
		system("color 2a");
		cout<<" "<<endl;
		cout<<" "<<endl;
		cout<<" "<<endl;
		cout<<""<<endl;
		cout<<"   ┌───────────────────────────────────┐\n";
		cout<<"   │                    ★★★★★图书管理系统★★★★★                  │\n";
		cout<<"   ├────────────────┬──────────────────┤\n";
		cout<<"   │          1:管理员登录          │              0:退出系统            │\n";
		cout<<"   ├────────────────┼──────────────────┤\n";
		cout<<"   │          请您选择:                                                   │\n";
		cout<<"   └───────────────────────────────────┘\n";
		cout<<"\t";
		cin>>h;
		if(h>=0&&h<2)
		{
			switch(h)
			{
			case 0:													//退出系统
				system("cls");
				system("color 2a");
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"    "<<endl;
				cout<<"\n\n\t\t\t谢谢光临本图书管理系统!"<<endl;
				exit(0);
			case 1: 
				login();											//登录
				system("pause");
				break;
			}
		}
		else 
		{
			system("cls");
			system("color 4e");
            cout<<"    "<<endl;
            cout<<"    "<<endl;
            cout<<"    "<<endl;
            cout<<"       "<<"\n\n\t\t\t您的操作是无效的！！！！"<<endl;
            system("pause");
		}
	}
}

#include "phone.h"
//判空
//空：1
//不空：wei0
int isContactEmpty(contact_p c)
{
    assert(c);

    return c->size == 0 ? 1 : 0;
}

//初始化
void initContact(contact_pp c)
{
    assert(c);

    //申请通讯录空间
    *c = (contact_p)malloc(sizeof(contact_t));
    if(NULL == *c){
        perror("malloc");
        exit(1);
    }

    //申请成员空间
    (*c)->contactList = (person_p)malloc(sizeof(person_t)*INITCAP);
    if(NULL == (*c)->contactList){
        perror("malloc");
        exit(2);
    }

    //初始化长度和容量
    (*c)->cap = INITCAP;
    (*c)->size = 0;

    fileLoad(*c);
}

//从文件读取通讯录信息
int fileLoad(contact_p c)
{
    FILE *fp;
    person_t p;
    int i = 0;

    assert(c);

    //打开文件
    fp = fopen(FILE_NAME,"rb");
    if(NULL == fp){
        perror("fopen");
        return -2;
    }

    while(1){
        //读取到p
        fread(&p, sizeof(person_t), 1, fp);
        if(0 != feof(fp)){
            break;
        }
        //将读取的添加到通讯录
        addContact(c, &p);
    }
    fclose(fp);

    return 0;
}


//判满
//满：1
//未满：0
static int isContactFull(contact_p c)
{
    assert(c);

    return c->size >= c-> cap ? 1 : 0;
}


//自增
static int incContact(contact_p c)
{
    person_p new_c = NULL;

    assert(c);

    //申请新空间
    new_c = (person_p)realloc(c->contactList, (c->cap+INCREMENT)*sizeof(person_t));
    if(NULL == new_c){
        perror("realloc");
        return 0;
    }

    //将新空间分配给旧空间
    c->contactList = new_c;
    c->cap += INCREMENT;
    printf("Is full, increse success!\n");
    return 1;
}

//添加联系人
void addContact(contact_p c, person_p p)
{
    assert(c);
    assert(p);

    //如果不空直接添加，如果空就先自增再添加
    if(!isContactFull(c) || incContact(c)){
        c->contactList[c->size] = *p;
        c->size++;
    }
}

//查找
//是空的：打印空
//没找到：返回 -1
//找到：返回下标
int searchContact(contact_p c, char *search_name)
{
    int i = 0;

    assert(c);
    assert(search_name);

    //通讯录是空的
    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
    }

    for(i = 0; i < c->size; i++){
        if( 0 == strcmp(c->contactList[i].name, search_name) ){
            break;
        }
    }

    //没找到
    if(i == c->size)
    {
        return -1;
    }

    //找到返回下标
    return i;


}


//按名字排序
void sortContact(contact_p c)
{
    int i = 0;
    int j = 0;
    int pos = 0;
    person_t temp;

    assert(c);

    //是空的就不用排
    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
        return ;
    }

    //不空用冒泡法按名字排序
    for(i = c->size-1; i>0; i--){
        for(j = 0; j<i; j++){
            if(1 == strcmp(c->contactList[j].name, c->contactList[j+1].name)){
                temp = c->contactList[j];
                c->contactList[j] = c->contactList[j+1];
                c->contactList[j+1] = temp;
                pos = 1;
            }
        }
        if(0 == pos)
        {

            break;
        }
    }

    printf("Done...\n");
}

//修改联系人
void modContact(contact_p c)
{
    char name[32];
    int i = 0;
    char key_mod[8];

    assert(c);


    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
        return ;
    }

    //输入要修改的人的名字
    printf("Please input the name whicth you want to modify:");
    scanf("%s", name);

    //查找要修改人的下标
    i = searchContact(c, name);
    if(-1 == i)
    {
        printf("Not find!\n");
        return ;
    }

    //打印信息
    printf("------------------------------------------------------------------------------\n");
    printf("| name      | sex  | age | phone           | adds                            |\n");
    printf("| %-10s| %-5s| %-4d| %-16s| %-32s|\n",c->contactList[i].name, \
	c->contactList[i].sex,c->contactList[i].age, \
	c->contactList[i].phone,c->contactList[i].adds);
    printf("------------------------------------------------------------------------------\n");

    //输入要修改的关键字
    printf("Please input the infor whicth you want to modify:");
    fflush(stdin);
    scanf("%s", key_mod);

    //寻找要修改的关键字

    //如果要修改名字
    if(0 == strcmp("name", key_mod)){
        printf("Please input new name:");
        fflush(stdin);//注意要清空输入缓冲区
        scanf("%s", c->contactList[i].name);
        printf("Done...\n");
        return ;
    }

    //如果要修改性别
    if(0 == strcmp("sex", key_mod)){
        printf("Please input new sex:");
        fflush(stdin);
        scanf("%s", c->contactList[i].sex);
        printf("Done...\n");
        return ;
    }

    //如果要修改年龄
    if(0 == strcmp("age", key_mod)){
        printf("Please input new age:");
        fflush(stdin);
        scanf("%d", &c->contactList[i].age);
        printf("Done...\n");
        return ;
    }

    //如果要修改电话号码
    if(0 == strcmp("phone", key_mod)){
        printf("Please input new phone:");
        fflush(stdin);
        scanf("%s", c->contactList[i].phone);
        printf("Done...\n");
        return ;
    }

    //如果要修改地址
    if(0 == strcmp("adds", key_mod)){
        printf("Please input new adds:");
        fflush(stdin);
        scanf("%s", c->contactList[i].adds);
        printf("Done...\n");
        return ;
    }

    //没有找到要修改的关键字
    printf("ERROR!!\n");
}

//删除
void delContact(contact_p c, char *del_name)
{
    int i = 0;
    int j = 0;
    assert(c);

    i = searchContact(c, del_name);
    if(-1 == i)
    {
        printf("Not find!\n");
        return ;
    }

    //将后面的成员前移
    for(j = i;j<c->size; j++){
        c->contactList[j] = c->contactList[j+1];
    }

    c->size--;
    printf("Done...\n");
}

//清空通讯录
void emptyContact(contact_p c)
{
    assert(c);

    //直接将当前大小置0
    c->size = 0;
    printf("Done...\n");
}

//打印信息
void showContact(contact_p c)
{
    int i = 0;

    assert(c);

    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
        return ;
    }

    printf("\ntotal:%d\n", c->size);//打印总人数

    //打印详细信息
    printf("------------------------------- contacts -------------------------------------\n");
    printf("| name      | sex  | age | phone           | adds                            |\n");
    printf("------------------------------------------------------------------------------\n");
    for(; i < c->size; i++){
        printf("| %-10s| %-5s| %-4d| %-16s| %-32s|\n",c->contactList[i].name, \
		c->contactList[i].sex,c->contactList[i].age, \
		c->contactList[i].phone,c->contactList[i].adds);
    }
    printf("------------------------------------------------------------------------------\n\n");
}

//帮助（使用手册）
void helpContact()
{
    printf("\n");
    printf("-----------------------------------  HELP  -----------------------------------\n");
    printf("| You could use this app store up some information about your friends.       |\n");
    printf("| 1、you can add information by choose ( 1 ).                                |\n");
    printf("| 2、you can delect someone by choose ( 2 ).                                 |\n");
    printf("| 3、you can search someone by choose ( 3 ).                                 |\n");
    printf("| 4、you can change some's information by choose ( 4 ).                      |\n");
    printf("| 5、you can show information list by choose ( 5 ).                          |\n");
    printf("| 6、you can empty all information by choose ( 6 ).                          |\n");
    printf("| 7、you can sort people by choose ( 7 ).                                    |\n");
    printf("------------------------------------------------------------------------------\n\n");
}



//储存到文件
int fileStore(contact_p c)
{
    FILE *fp;
    int i = 0;

    assert(c);

    //打开文件
    fp = fopen(FILE_NAME,"wb");
    if(NULL == fp){
        perror("fopen");
        return -1;
    }

    //一次储存一个人大小的内容
    for(; i<c->size; i++){
        fwrite(c->contactList+i, sizeof(person_t), 1, fp);
    }

    fclose(fp);

    return 0;
}

//摧毁通讯录
void destroyContact(contact_p c)
{
    assert(c);

    fileStore(c);
    free(c->contactList);//释放成员
    c->contactList = NULL;
    free(c);//释放整体
    c = NULL;
}

//退出
void exitContact(contact_p c)
{
    int n = 0;
    printf("\nAre you sure quit ?\n");
    while(1)
    {
        printf("----------------\n");
        printf("| 1.YES | 2.NO |\n");
        printf("----------------\n");

        scanf("%d", &n);
        if( 1 == n)
        {
            destroyContact(c);
            exit(0);
        }
        else if( 2 == n )
        {
            return;
        }
        else
        {
            printf("Error ！ \n");
        }
    }

}
//菜单
static void meun(){
    printf("\n");
    printf("欢迎......\n");
    printf("-----------------------------------  CONTACTS  --------------------------------\n");
    printf("--           1.添加                                2.删除                   --\n");
    printf("--           3.查找                             4.退出                     --\n");
    printf("--           5.查看                               6.清除                    --\n");
    printf("--           7.排序                               8.帮助                     --\n");
    printf("--           0.退出                                                          --\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("\n");
}

//添加
static void myAdd(contact_p c){
    person_t p;

    assert(c);

    printf("Please input information<name,sex,age,phone,addr>:\n");
    scanf("%s %s %d %s %s", p.name, p.sex, &p.age, p.phone, p.adds);
    addContact(c, &p);
}

//删除
static void myDel(contact_p c){
    char del_name[32];
    int n = 0;
    assert(c);

    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
        return ;
    }

    printf("Please input the name whicth you want delect:");
    scanf("%s", del_name);
    printf("\nAre you sure delect: %s ?\n", del_name);
    while(1)
    {
        printf("----------------\n");
        printf("| 1.YES | 2.NO |\n");
        printf("----------------\n");

        scanf("%d", &n);
        if( 1 == n)
        {
            delContact(c, del_name);
            return ;
        }
        else if( 2 == n )
        {
            return;
        }
        else
        {
            printf("Error ！ \n");
        }
    }

}

//查找
static void mySearch(contact_p c)
{
    char search_name[32];
    int pos = 0;
    int i = 0;

    assert(c);

    if(isContactEmpty(c)){
        printf("Contact is empty!\n");
        return ;
    }

    printf("Please input rhe name whicth you want to search:");
    scanf("%s", search_name);
    pos = searchContact(c, search_name);
    if(-1 == pos){
        printf("Not find!\n");
    }
    else{

        printf("------------------------------------------------------------------------------\n");
        printf("| name      | sex  | age | phone           | adds                            |\n");
        printf("| %-10s| %-5s| %-4d| %-16s| %-32s|\n",c->contactList[i].name, \
		c->contactList[i].sex,c->contactList[i].age, \
		c->contactList[i].phone,c->contactList[i].adds);
        printf("------------------------------------------------------------------------------\n");
    }
}

int main()
{
    int s = -1;//选择控制变量
    contact_p myContact = NULL;//声明通讯录
    system("color 2");
    initContact(&myContact);//初始化
    while(1){
        meun();
        printf("Please select<0~7>:");
        fflush(stdin);
        scanf("%d", &s);
        switch(s){
            case 1:system("cls");
                myAdd(myContact);//添加联系人
                system("pause");//可以实现冻结屏幕，便于观察程序的执行结果
                break;
            case 2:system("cls");//可以实现清屏操作
                myDel(myContact);//删除联系人
                system("pause");
                break;
            case 3:system("cls");
                mySearch(myContact);//查找并打印联系人
                system("pause");
                break;
            case 4:system("cls");
                modContact(myContact);//修改联系人
                system("pause");
                break;
            case 5:system("cls");
                showContact(myContact);//显示联系人列表
                system("pause");
                break;
            case 6:system("cls");
                emptyContact(myContact);//清空通讯录
                system("pause");
                break;
            case 7:system("cls");
                sortContact(myContact);//排序通讯录
                system("pause");
                break;
            case 8:system("cls");
                helpContact();//帮助
                system("pause");
                break;
            case 0:system("cls");
                exitContact(myContact);//退出
                break;
            default:printf("ERROR SELECTION !!!\n");//错误选择
                system("pause");
                break;
        }
        system("cls");//清屏
    }

    return 0;
}
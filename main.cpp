#include <iostream> // cout, endl
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <cmath>
#include <ctime> // clock_t, clock, CLOCKS_PER_SEC
#include <stack> // push, pop, top,empty
#include <algorithm> // sort
#include <sstream> // 10 16

using namespace std;
ifstream fs ;

class Table {
public :
    vector<string> ins;
};
class Word {
 public :
    string oneword;
    int tablenum;
    int addr ;
};

class Line{
    public:
    string aline;
    vector<Word> tokenlist;
};

class package  {
    public:
    string label ;                  // 存label
    string ins ;
    vector<string> group1_List ;            // 存r1
    vector<string> group2_List ;        // 存r2
    string trash;               //存註解
    int g1table ;
    int g1addr  ;
    int g2table ;
    int g2addr  ;
    string aline    ;               //存原始資料
    int format  ;
    string immed = "0";         //  #判斷
    string indirect = "0" ;     //  @判斷
};

class sym{
 public:
    string addr;
    string word;
};

class obj {
 public:
    string objcode;
    string tokenline;
    string loc;
    string error = "\0";
    int format = 0;
};

class Token {
public:
    vector<Table> tablelist;
    vector<Line> wordlist;
    void prepare( ){
        Table data;
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
        tablelist.push_back( data);
    }


    void gettable( int num ) {
        string str ;
        tablelist.at(num).ins.clear();
        tablelist.at(num).ins.push_back(str);
        while( getline( fs, str ) ) {

            for( int  i = 0; i <str.size() ; i++ ) {
                if( str[i] == ' '|| str[i] == '\t'|| str[i] == '\n' ) {
                    str = str.substr( 0 , i );
                }
            }

            str = bigword(str);
            tablelist.at(num).ins.push_back(str);

        } // while

    }
    void show_ins() {
        for(int i  = 0 ; i < tablelist.size() ;i++) {
            for(int k = 0 ; k < tablelist.at(i).ins.size() ; k++ ){
                cout << tablelist.at(i).ins.at(k) << endl ;
            }
            cout <<endl;
        }
    }
    //==============================================================token

    void gettoken() {
        string str ;
        Line temp;
        while( getline( fs, str, '\n') ) {
            //str = bigword(str);
            temp.aline = str;
            wordlist.push_back( temp );
        } // while
    }
    string bigword( string  str ) {
        string newone;
        char ch;

        for( int i = 0 ;i< str.size() ; i++) {
            if ( (int)str[i]  >= 'a' &&  (int)str[i] <= 'z' ) {
                ch = str[i] - 32;
                newone  += ch;
            }
            else {
                newone += str[i];

            }
        }

        return newone;
    }

    void findplace( string target , int  & tnum ,int & anum ) {
        string temp = "\0";
        int loc = 0;
        int tmp;
        target = bigword( target );
        for( int i = 0 ; i < 5 ; i++  ) {
            for ( int k = 0; k < tablelist.at(i).ins.size()  ; k++ ) {
                temp = tablelist.at(i).ins.at(k);
               if(  target.compare( temp ) == 0 ) {
                    anum = k;
                    tnum = i;
                    return;
               }
            }
        }

        for ( int i = 0 ; i < target.size() - 1 ;  i++) {
            tmp = (int)target[i];
            if ( tmp >= '0' && tmp <= '9'  )     {

            }

            else if ( tmp >= 'A' &&  tmp <= 'F' ){

            }
            else {
                inserttable( target, 5,  loc );
                anum = loc;
                tnum = 5;
                return;
            }
        }

        tnum = 6;
        inserttable( target, 6,  loc );
        anum = loc;
    }

    void inserttable( string str , int table , int & loc ) {
        int asc ;
        for( int i = 0; i < str.size(); i++ ) {
            asc = int( str[i] ) + asc;
        }
        asc = asc % 100;
        if( tablelist.at(table).ins.at(asc) == "\0" ) {
            tablelist.at(table).ins.at(asc) = str;
            loc = asc;
        }
        else if ( tablelist.at(table).ins.at(asc).compare( str ) == 0 ) {
            loc = asc;
        }
        else {
            asc = asc + 1;
            if( asc >= 100 ) {
                asc = 0;
            }
            while ( tablelist.at(table).ins.at(asc) !="\0" ) {
                if ( tablelist.at(table).ins.at(asc).compare( str ) == 0 ) {
                    loc = asc;
                    return;
                }
                asc = asc + 1;
                if( asc >= 100 ) {
                    asc = 0;
                }
            }
            tablelist.at(table).ins.at(asc) = str;
            loc = asc;
        }
    }

    void cuttoken() {
        string temp;
        Word target ;
        int anum = 0;
        int tnum = 0;
        char ch ;
        tablelist.at(5).ins.resize(100) ;
        tablelist.at(6).ins.resize(100) ;
        tablelist.at(7).ins.resize(100) ;

        for( int i = 0; i < wordlist.size() ; i++  ) {
            int k = 0 ;
            bool sixteen = false;
            target.oneword = "\0";
            target.addr = 0;
            target.tablenum = 0;
            while( k < wordlist.at(i).aline.size()  ) {
                temp = bigword ( wordlist.at(i).aline );
                if( temp[k] == ','|| temp[k] == '+' || temp[k] == '-' || temp[k] == '*'
                    || temp[k] == '/' || temp[k] == ':' || temp[k] == ';'
                    || temp[k] == '?' || temp[k] == '\''|| temp[k] == '.'
                    || temp[k] == '=' || temp[k] == '#' || temp[k] == '@' ) {
                    if ( target.oneword !="\0") {
                        if (target.oneword == "C"  && temp[k] == '\'' ) {
                            target.oneword = "\0";
                            target.addr = 0;
                            target.tablenum = 0;
                        }
                        else if ( target.oneword == "X"  && temp[k] == '\'' ) {
                            target.oneword = "\0";
                            sixteen = true;
                            target.addr = 0;
                            target.tablenum = 0;
                        }


                        else {
                            findplace( target.oneword , tnum , anum  );
                            target.addr = anum;
                            target.tablenum = tnum;
                            wordlist.at(i).tokenlist.push_back( target );
                            target.oneword = "\0";
                            target.addr = 0;
                            target.tablenum = 0;
                        }
                    }

                        target.oneword += temp[k];
                        findplace( target.oneword , tnum , anum  );  // 輸出第一個delimeter
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target );
                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;

                    if ( temp[k] == '\'' ) {
                        k++;
                        while ( temp[k] != '\''  ) {
                            target.oneword += temp[k];
                            k++;
                        }
                        if ( sixteen == true ) {
                          findplace( target.oneword , tnum , anum  );
                          sixteen == false;
                        }

                        else {
                          inserttable( target.oneword , 7 , anum );
                          tnum = 7;
                        }
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target );  // 輸出string 內容;
                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;

                        target.oneword += temp[k];
                        findplace( target.oneword , tnum , anum  );     // 輸出 ' 內容;
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target );

                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;
                        k++;

                    }

                    else if( temp[k] == '(' ) {
                        k++;
                        while ( temp[k] != '('  ) {
                            target.oneword += temp[k];
                            k++;
                        }
                        findplace( target.oneword , tnum , anum  );
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target);  // 輸出string 內容
                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;
                        target.oneword += temp[k];
                        findplace( target.oneword , tnum , anum  );
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target );
                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;
                        k++;
                    }

                    else if ( temp[k] == '.') {
                        break;
                    }

                    else {
                        k++;
                    }
                }

                else if ( temp[k] == ' ' || temp[k] == '\t' ) {
                    if ( target.oneword != "\0" ) {
                        findplace( target.oneword , tnum , anum  );
                        target.addr = anum;
                        target.tablenum = tnum;
                        wordlist.at(i).tokenlist.push_back( target );
                        target.oneword = "\0";
                        target.addr = 0;
                        target.tablenum = 0;
                    }
                    else {
                        k++;
                    }
                }

                else {
                    target.oneword+= temp[k];
                    k++;
                }
            }

            if ( target.oneword != "\0" ) {
                findplace( target.oneword , tnum , anum  );
                target.addr = anum;
                target.tablenum = tnum;
                wordlist.at(i).tokenlist.push_back( target );
                target.oneword = "\0";
                target.addr = 0;
                target.tablenum = 0;
            }
        }
    }

    void showtoken() {
        for (int k = 0; k < wordlist.size() ; k++ ) {
            cout << wordlist.at(k).aline << endl ;

            for(int i = 0; i < wordlist.at(k).tokenlist.size() ; i++  ){
                cout << '(' << wordlist.at(k).tokenlist.at(i).tablenum << ',' << wordlist.at(k).tokenlist.at(i).addr <<')' ;
            }

            cout << endl;
        }
    }

    void outfile(){
        ofstream out ;
        out.open( "x86_output.txt" );
        for (int k = 0; k < wordlist.size() ; k++ ) {
            out << wordlist.at(k).aline << "\n" ;

            for(int i = 0; i < wordlist.at(k).tokenlist.size() ; i++  ){
                out << '(' << wordlist.at(k).tokenlist.at(i).tablenum << ',' << wordlist.at(k).tokenlist.at(i).addr <<')' ;
            }
            out << "\n";
        }
        out.close();
    }

    //==============================================================================================================================================//syntex
    vector<package> packageList ;
    void makegroup() {
        package target;
        bool existg2 = false;
        for (int k = 0; k < wordlist.size() ; k++ ) {
            if ( wordlist.at(k).aline == "\0" ) {


            }
            else {
                for(int i = 0; i < wordlist.at(k).tokenlist.size() ; i++  ) {       // ex: COPY	START	1000

                    if ( wordlist.at(k).tokenlist.at(i).tablenum == 1 || wordlist.at(k).tokenlist.at(i).tablenum == 2 )  {    //table 1 ins
                        target.ins = wordlist.at(k).tokenlist.at(i).oneword ;
                    }

                    else if ( wordlist.at(k).tokenlist.at(i).tablenum == 3  ) {     // register  table3                //maybe 1, or g1,g2
                        if ( existg2 == false) {
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                            target.g1addr =wordlist.at(k).tokenlist.at(i).addr;
                        }
                        else {
                           target.group2_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                           target.g2addr = wordlist.at(k).tokenlist.at(i).addr;
                        }
                    }
                    else if ( wordlist.at(k).tokenlist.at(i).tablenum == 4  ) {     // delimiter table 4
                        if ( wordlist.at(k).tokenlist.at(i).oneword == "," ) {
                            existg2 = true;
                        }
                        else if ( wordlist.at(k).tokenlist.at(i).oneword == "." ) {
                            target.trash = wordlist.at(k).aline;
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                        }

                        else if ( wordlist.at(k).tokenlist.at(i).oneword == "#" ) {
                            target.immed = "1";
                        }
                        else if ( wordlist.at(k).tokenlist.at(i).oneword == "@" ) {
                            target.indirect = "1";
                        }
                        else if ( wordlist.at(k).tokenlist.at(i).oneword == "+" ) {
                            target.format = 4;
                        }
                        else {
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                        }
                    }

                    else if ( wordlist.at(k).tokenlist.at(i).tablenum == 5  ) {     // delimiter table 5 symbol         //maybe 1, or g1,g2
                        if ( i == 0 ) {
                            target.label = wordlist.at(k).tokenlist.at(i).oneword ;
                        }
                        else {
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                        }
                    }

                    else if ( wordlist.at(k).tokenlist.at(i).tablenum == 6  ) {     //  table 6  number        //maybe r g1,g2
                        if ( existg2 == false) {
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                            target.g1table = 6;
                        }
                        else{
                            target.group2_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                            target.g2table = 6;
                        }
                    }

                    else if ( wordlist.at(k).tokenlist.at(i).tablenum == 7  ) {     //  table 7 string         //maybe  g1,g2
                        if ( existg2 == false) {
                            target.group1_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                            target.g1table = 7;
                        }
                        else{
                            target.group2_List.push_back( wordlist.at(k).tokenlist.at(i).oneword );
                            target.g2table = 7;
                        }
                    }

                    else {
                        cout <<"error"<< endl;
                    }

                    target.aline = wordlist.at(k).aline;
                }

                if( target.ins == "\0" && target.trash == "\0" ) {
                    cout << "error no ins "<< endl;
                }


                packageList.push_back(target);
                target.label = "\0" ;
                target.ins = "\0" ;
                target.group1_List.clear() ;
                target.group2_List.clear();
                target.trash = "\0";
                target.g1table = 0;
                target.g2table =0;
                target.aline = "\0";
                target.format = 0;
                target.g2addr = 0;
                target.g1addr = 0;
                target.immed = "\0";
                target.indirect = "\0";
                existg2 = false;
            }
        }
    }
    void showallgroup() {

        for ( int k = 0; k < packageList.size() ; k++ ) {
            cout <<"label   "<< packageList.at(k).label << " ";
            cout <<"ins "<< packageList.at(k).ins << " ";
            cout <<"g1  " ;
            for ( int i = 0; i < packageList.at(k).group1_List.size() ; i++ ) {
                if ( packageList.at(k).group1_List.at(i) == "." ) {
                   cout << "trash"<<packageList.at(k).trash ;
                }
                else {
                   cout << packageList.at(k).group1_List.at(i);
                }
            }
            cout << "g2 ";
            for ( int j = 0; j < packageList.at(k).group2_List.size() ; j++ ) {
                cout << packageList.at(k).group2_List.at(j);
            }
           cout << endl;
        }

    }

    string findopcode( string temp , int & format ) {
        string opcode = "\0";
        if( temp.compare( "ADD" ) == 0 ) {
            opcode = "18";
            format = 3;
        }
        else if( temp.compare( "AND" ) == 0 ) {
            opcode = "40";
            format = 3;
        }
        else if( temp.compare( "COMP" ) == 0 ) {
            opcode = "28";
            format = 3;
        }
        else if( temp.compare( "DIV" ) == 0 ) {
            opcode = "24";
            format = 3;
        }
        else if( temp.compare( "J" ) == 0 ) {
            opcode = "3C";
            format = 3 ;
        }
        else if( temp.compare( "JEQ" ) == 0 ) {
            opcode = "30";
            format = 3;
        }
        else if( temp.compare( "JGT" ) == 0 ) {
            opcode = "34";
            format = 3;
        }
        else if( temp.compare( "JLT" ) == 0 ) {
            opcode = "38";
            format = 3;
        }
        else if( temp.compare( "JSUB" ) == 0 ) {
            opcode = "48";
            format = 3;
        }
        else if( temp.compare( "LDA" ) == 0 ) {
            opcode = "00";
            format = 3;
        }
        else if( temp.compare( "LDCH" ) == 0 ) {
            opcode = "50";
            format = 3;
        }
        else if( temp.compare( "LDL" ) == 0 ) {
            opcode = "08";
            format = 3;
        }
        else if( temp.compare( "LDX" ) == 0 ) {
            opcode = "04";
            format = 3;
        }
        else if( temp.compare( "MUL" ) == 0 ) {
            opcode = "20";
            format = 3;
        }
        else if( temp.compare( "OR" ) == 0 ) {
            opcode = "44";
            format = 3;
        }
        else if( temp.compare( "RD" ) == 0 ) {
            opcode = "D8";
            format = 3;
        }
        else if( temp.compare( "RSUB" ) == 0 ) {
            opcode = "4C";
            format = 3;
        }
        else if( temp.compare( "STA" ) == 0 ) {
            opcode = "0C";
            format = 3;
        }
        else if( temp.compare( "STCH" ) == 0 ) {
            opcode = "54";
            format = 3;
        }
        else if( temp.compare( "STL" ) == 0 ) {
            opcode = "14";
            format = 3;
        }
        else if( temp.compare( "STSW" ) == 0 ) {
            opcode = "E8";
            format = 3;
        }
        else if( temp.compare( "STX" ) == 0 ) {
            opcode = "10";
            format = 3;
        }
        else if( temp.compare( "SUB" ) == 0 ) {
            opcode = "1C";
            format = 3;
        }
        else if( temp.compare( "TD" ) == 0 ) {
            opcode = "E0";
            format = 3;
        }
        else if( temp.compare( "TIX" ) == 0 ) {
            opcode = "2C";
            format = 3;
        }
        else if( temp.compare( "WD" ) == 0 ) {
            opcode = "DC";
            format = 3;
        }

        //=================================================================================SICXE
        else if( temp.compare( "ADDF" ) == 0 ) {
            opcode = "58";
            format = 3;
        }
        else if( temp.compare( "ADDR" ) == 0 ) {
            opcode = "90";
            format = 2;
        }
        else if( temp.compare( "CLEAR" ) == 0 ) {
            opcode = "B4";
            format = 2;
        }
        else if( temp.compare( "COMPF" ) == 0 ) {
            opcode = "88";
            format = 3;
        }
        else if( temp.compare( "COMPR" ) == 0 ) {
            opcode = "A0";
            format = 2;
        }
        else if( temp.compare( "DIVF" ) == 0 ) {
            opcode = "64";
            format = 3;
        }
        else if( temp.compare( "DIVR" ) == 0 ) {
            opcode = "9C";
            format = 2;
        }
        else if( temp.compare( "FIX" ) == 0 ) {
            opcode = "C4";
            format = 1;
        }
        else if( temp.compare( "FLOAT" ) == 0 ) {
            opcode = "C0";
            format = 1;
        }
        else if( temp.compare( "HIO" ) == 0 ) {
            opcode = "F4";
            format = 1;
        }
        else if( temp.compare( "LDB" ) == 0 ) {
            opcode = "68";
            format = 3;
        }
        else if( temp.compare( "LDF" ) == 0 ) {
            opcode = "70";
            format = 3;
        }
        else if( temp.compare( "LDS" ) == 0 ) {
            opcode = "6C";
            format = 3;
        }
        else if( temp.compare( "LDT" ) == 0 ) {
            opcode = "74";
            format = 3;
        }
        else if( temp.compare( "LPS" ) == 0 ) {
            opcode = "D0";
            format = 3;
        }
        else if( temp.compare( "MULF" ) == 0 ) {
            opcode = "60";
            format = 3;
        }
        else if( temp.compare( "MULR" ) == 0 ) {
            opcode = "98";
            format = 2;
        }
        else if( temp.compare( "NORM" ) == 0 ) {
            opcode = "C8";
            format = 1;
        }
        else if( temp.compare( "RMO" ) == 0 ) {
            opcode = "AC";
            format = 2;
        }
        else if( temp.compare( "SHIFTL" ) == 0 ) {
            opcode = "A4";
            format = 2;
        }
        else if( temp.compare( "SHIFTR" ) == 0 ) {
            opcode = "A8";
            format = 2;
        }
        else if( temp.compare( "SIO" ) == 0 ) {
            opcode = "F0";
            format = 1;
        }
        else if( temp.compare( "SSK" ) == 0 ) {
            opcode = "EC";
            format = 3;
        }
        else if( temp.compare( "STB" ) == 0 ) {
            opcode = "78";
            format = 3;
        }
        else if( temp.compare( "STF" ) == 0 ) {
            opcode = "80";
            format = 3;
        }
        else if( temp.compare( "STI" ) == 0 ) {
            opcode = "D4";
            format = 3;
        }
        else if( temp.compare( "STS" ) == 0 ) {
            opcode = "7C";
            format = 3;
        }
        else if( temp.compare( "STT" ) == 0 ) {
            opcode = "84";
            format = 3;
        }
        else if( temp.compare( "SUBF" ) == 0 ) {
            opcode = "5C";
            format = 3;
        }
        else if( temp.compare( "SUBR" ) == 0 ) {
            opcode = "94";
            format = 2;
        }
        else if( temp.compare( "SVC" ) == 0 ) {
            opcode = "B0";
            format = 2;
        }
        else if( temp.compare( "TIO" ) == 0 ) {
            opcode = "F8";
            format = 1;
        }
        else if( temp.compare( "TIXR" ) == 0 ) {
            opcode = "B8";
            format = 2;
        }

        else {
            cout << "error ins :"<< temp <<endl;
        }
        return opcode;
    }


    string ten2sixteen( int ten ) {
        string s1;
        stringstream ss;
        ss<<hex<<ten;         //10進位制轉成十六進位制讀入流中，，再以字串輸出
        ss>>s1;
        ss.clear();

        return s1;
    }

    int sixteen2ten( string six ) {
        int a;
        stringstream ss;
        ss<<hex<<six;    //以16進位制讀入流中
        ss>>a;        //10進位制int型輸出
        ss.clear();
        return a ;
    }

    string givezero( string str ,int num ) {
        int k = 0;
        string ans = "\0";
        k = num - str.size() ;
        while ( k > 0 ) {
            ans += "0";
            k--;
        }
        return ans;
    }


    vector<sym>symtab;
    vector<obj>objlist;
    void makesic() {
        int trash ;
        int locationC = 0;
        bool start  = false ;
        sym tempsym;
        obj tempobj;
        string tempgroup1;
        string tempgroup2;
        bool existsym = false;
        for ( int k = 0; k < packageList.size() ; k++ ) {

            if ( packageList.at(k).label != "\0" ) {            // have label  ===> addr
                for (int i = 0; i < symtab.size() ; i ++ ) {                        //search table

                    if ( packageList.at(k).label == symtab.at(i).word ) {           // exist
                        if ( symtab.at(i).addr == "\0" ) {                   // check addr
                            if ( locationC == 0 ) {                                                                 // not start
                                if ( packageList.at(k).ins == "START" ) {
                                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                                        tempgroup1 += packageList.at(k).group1_List.at(i);
                                    }
                                locationC = sixteen2ten( tempgroup1 );
                                }
                            }
                            symtab.at(i).addr = ten2sixteen ( locationC );
                            symtab.at(i).addr = ( givezero( symtab.at(i).addr , 4 ) += symtab.at(i).addr ) ;
                        }
                        else {
                            symtab.at(i).addr = ten2sixteen ( locationC );
                            symtab.at(i).addr = ( givezero( symtab.at(i).addr , 4 ) += symtab.at(i).addr ) ;
                        }
                        existsym = true;
                    }

                }

                if ( existsym != true ) {
                    if ( locationC == 0 ) {                                                  // not start
                        if ( packageList.at(k).ins == "START" ) {
                            for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                                tempgroup1 += packageList.at(k).group1_List.at(i);
                            }
                            locationC = sixteen2ten( tempgroup1 );
                        }

                        else {
                            cout << " error symbol no start " << endl;
                        }
                    }

                    tempsym.addr = ten2sixteen ( locationC );
                    tempsym.word = packageList.at(k).label ;
                    symtab.push_back( tempsym ) ;
                    tempsym.word = "\0" ;
                    tempsym.addr = "\0" ;
                }
                existsym = false;
                tempgroup1 = "\0";
                //================================================================

                if( packageList.at(k).ins != "\0" ) {
                    if ( packageList.at(k).ins == "START" ) {
                        start = true;
                    }
                    else if ( packageList.at(k).ins == "END" ){ }
                    else if ( packageList.at(k).ins == "BYTE" ){ }
                    else if ( packageList.at(k).ins == "WORD" ){ }
                    else if ( packageList.at(k).ins == "RESB") { }
                    else if ( packageList.at(k).ins == "RESW" ){ }
                    else if ( packageList.at(k).ins == "EQU" ){ }
                    else if ( packageList.at(k).ins == "BASE" ){ }
                    else if ( packageList.at(k).ins == "LTORG" ){ }
                    else  {
                        tempobj.objcode = findopcode ( packageList.at(k).ins , trash );
                    }
                }

                if ( packageList.at(k).group1_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                        tempgroup1 += packageList.at(k).group1_List.at(i);
                    }
                    if ( packageList.at(k).ins == "START" ) {
                        locationC = sixteen2ten( tempgroup1 );
                    }

                    if ( packageList.at(k).ins == "WORD" ){                // WORD
                        tempobj.objcode = givezero( ten2sixteen( stoi( tempgroup1) ), 6 );
                        tempobj.objcode += ten2sixteen( stoi( tempgroup1  ) ) ;
                    }

                    else if( packageList.at(k).ins == "BYTE" ) {
                        int asc = 0;
                        if ( packageList.at(k).g1table == 6 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    tempobj.objcode += tempgroup1[i]  ;
                                }
                            }
                        }

                        else if ( packageList.at(k).g1table == 7 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    asc = (int)tempgroup1[i] ;
                                    tempobj.objcode += ten2sixteen( asc ) ;
                                    asc = 0;
                                }
                            }
                        }
                    }

                    else {                                          // not start
                    }
                }
                if ( packageList.at(k).group2_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group2_List.size() ; i ++ ){
                            tempgroup2 += packageList.at(k).group2_List.at(i);
                    }

                    if ( tempgroup2 == "X" ) {  }
                    else { }

                }

            }

            //====================================================================================
            else {                                                                                        // no label
                if( packageList.at(k).ins != "\0" ) {
                    if ( packageList.at(k).ins == "START" ) {
                        start = true;
                    }
                    else if ( packageList.at(k).ins == "END" ){ }
                    else if ( packageList.at(k).ins == "BYTE" ){ }
                    else if ( packageList.at(k).ins == "WORD" ){ }
                    else if ( packageList.at(k).ins == "RESB") { }
                    else if ( packageList.at(k).ins == "RESW" ){ }
                    else if ( packageList.at(k).ins == "EQU" ){}
                    else if ( packageList.at(k).ins == "BASE" ){}
                    else if ( packageList.at(k).ins == "LTORG" ){}
                    else if (packageList.at(k).ins == "RSUB" )   {
                        tempobj.objcode = findopcode ( packageList.at(k).ins ,trash);
                        tempobj.objcode  += givezero(  tempobj.objcode , 6 );
                    }
                    else  {
                        tempobj.objcode = findopcode ( packageList.at(k).ins,trash );
                    }
                }

                if ( packageList.at(k).group1_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){               // 先合併
                        tempgroup1 += packageList.at(k).group1_List.at(i);
                    }
                    if ( packageList.at(k).ins == "START" ) {
                        locationC = sixteen2ten( tempgroup1 );
                    }
                    else if ( packageList.at(k).ins == "WORD" ){                // WORD
                        tempobj.objcode = givezero( ten2sixteen( stoi( tempgroup1) ), 6 );
                        tempobj.objcode += ten2sixteen( stoi( tempgroup1  ) ) ;
                    }

                    else if( packageList.at(k).ins == "BYTE" ) {
                        int asc = 0;
                        if ( packageList.at(k).g1table == 6 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    tempobj.objcode += tempgroup1[i]  ;
                                }
                            }
                        }

                        else if ( packageList.at(k).g1table == 7 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    asc = (int)tempgroup1[i] ;
                                    tempobj.objcode += ten2sixteen( asc ) ;
                                    asc = 0;
                                }
                            }
                        }
                    }

                    else { }
                }
                if ( packageList.at(k).group2_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group2_List.size() ; i ++ ) {
                        tempgroup2 += packageList.at(k).group2_List.at(i);
                    }
                }
            }

            tempobj.tokenline = packageList.at(k).aline ;
            tempobj.loc = ten2sixteen( locationC );  //================================

            objlist.push_back( tempobj ) ;

            if ( packageList.at(k).ins == "BYTE" ) {
                if (packageList.at(k).g1table == 6 )  {                     // 'int'
                    locationC = locationC +  (packageList.at(k).group1_List.size() / 2 )  ;
                }

                else if (packageList.at(k).g1table == 7 )  {   //"string"
                    locationC = locationC  +  ( packageList.at(k).group1_List.size() )  ;
                }
            }
            else if ( packageList.at(k).ins == "RESB" ) {
                int i = stoi( tempgroup1 ) ;
                locationC = locationC + i ;
            }
            else if ( packageList.at(k).ins == "START" ) {}
            else if ( packageList.at(k).ins == "END" ) {}
            else if ( tempgroup1 == "." ) { }
            else {
                locationC = locationC + 3;
            }

            tempgroup1 = "\0";
            tempgroup2 = "\0";
            tempobj.loc = "\0";
            tempobj.objcode = "\0";
            tempobj.tokenline = "\0";

        }
        if ( start == false ) {
            cout << " error no start" << endl;
        }

        for( int k = 0; k< packageList.size() ; k++ ) {

            for( int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                tempgroup1 += packageList.at(k).group1_List.at(i);
            }
            if ( packageList.at(k).group1_List.size() != 0 ) {
                if ( packageList.at(k).group2_List.size() == 0 )  {
                    for( int i = 0; i < symtab.size() ; i++ ) {
                        if ( tempgroup1  == symtab.at(i).word ) {
                            objlist.at(k).objcode  += symtab.at(i).addr;
                        }
                    }
                }
            }


            if ( packageList.at(k).group2_List.size() != 0 ) {
                for(int i = 0; i < packageList.at(k).group2_List.size() ; i ++ ){
                        tempgroup2 += packageList.at(k).group2_List.at(i);
                }

                if ( tempgroup2 == "X" ) {
                    for( int i = 0; i < symtab.size() ; i++ ) {
                        if ( tempgroup1  == symtab.at(i).word ) {
                            int big = 32768;
                            big = big + sixteen2ten( symtab.at(i).addr ) ;
                            objlist.at(k).objcode  += ten2sixteen( big );
                            big = 0;
                        }
                    }
                }
            }

            if ( packageList.at(k).ins == "END" ) {
                cout << setw(3) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else {
                cout << setw(3) << 5*k + 5 << "\t"<< bigword ( objlist.at(k).loc )  << "\t" << objlist.at(k).tokenline << "\t\t\t" << bigword ( objlist.at(k).objcode )  <<endl  ;
            }
            tempgroup1 = "\0";
            tempgroup2 = "\0";
        }

        for( int k = 0; k < symtab.size() ; k++ ) {
            cout << symtab.at(k).addr << "\t" << symtab.at(k).word << endl;
        }

    }

    void outfile2() {

        ofstream out ;
        out.open( "my_sic_output.txt" );
        out << "Line\tLoc\tSource statement\t\tObject code" << endl ;
        out << endl;
        for( int k = 0; k< packageList.size() ; k++ ) {
            if ( packageList.at(k).ins == "END" ) {
                out << setw(3) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else if (packageList.at(k).trash != "\0") {
                out << setw(3) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else {
                out << setw(3) << 5*k + 5 << "\t"<< bigword ( objlist.at(k).loc )  << "\t" << objlist.at(k).tokenline << "\t\t\t" << bigword ( objlist.at(k).objcode )  <<endl  ;
            }
        }
        out.close();
    }
    //========================================================================================================================================================SICXE
    //========================================================================================================================================================SICXE
    //========================================================================================================================================================SICXE
    //========================================================================================================================================================SICXE
    //========================================================================================================================================================SICXE

    int bin2ten( string str ) {
        int num = 0 ;
        for ( int i = 0 ; i < str.size() ; i++ ) {
            num = 2*( num ) + ( str[i] - '0' )  ;
        }
        return num ;
    }

    string hex2bin ( string str ){
        string bin = "\0";
        for( int i = 0 ; i < str.size() ; i ++ ) {
            if( str[i] == '0'){
                bin = bin + "0000";
            }
            else if( str[i] == '1') {
                bin = bin + "0001";
            }
            else if( str[i] == '2') {
                bin = bin + "0010";
            }
            else if( str[i] == '3') {
                bin = bin + "0011";
            }
            else if( str[i] == '4') {
                bin = bin + "0100";
            }
            else if( str[i] == '5') {
                bin = bin + "0101";
            }
            else if( str[i] == '6') {
                bin = bin + "0110";
            }
            else if( str[i] == '7') {
                bin = bin + "0111";
            }
            else if( str[i] == '8') {
                bin = bin + "1000";
            }
            else if( str[i] == '9') {
                bin = bin + "1001";
            }
            else if( str[i] == 'a') {
                bin = bin + "1010";
            }
            else if( str[i] == 'b') {
                bin = bin + "1011";
            }
            else if( str[i] == 'c') {
                bin = bin + "1100";
            }
            else if( str[i] == 'd') {
                bin = bin + "1101";
            }
            else if( str[i] == 'e') {
                bin = bin + "1110";
            }
            else if( str[i] == 'f') {
                bin = bin + "1111";
            }

        }
        return bin;
    }

    int pc = 0;

    void makesicxe() {
        int locationC = 0;
        bool start  = false ;
        sym tempsym;
        obj tempobj;
        string tempgroup1;
        string tempgroup2;
        bool existsym = false;
        int format = 0;
        for ( int k = 0; k < packageList.size() ; k++ ) {

            if ( packageList.at(k).label != "\0" ) {            // have label  ===> addr

                if ( packageList.at(k).ins == "EQU" ) {
                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                        tempgroup1 += packageList.at(k).group1_List.at(i);
                    }
                    int asc = 0 ;
                    asc = stoi( tempgroup1 );
                    locationC = sixteen2ten ( ten2sixteen( asc ) );
                }
                for (int i = 0; i < symtab.size() ; i ++ ) {                        //search table


                    if ( packageList.at(k).label == symtab.at(i).word ) {           // exist
                        if ( symtab.at(i).addr == "\0" ) {                   // check addr
                            if ( locationC == 0 ) {                                                  // not start
                                if ( packageList.at(k).ins == "START" ) {
                                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                                        tempgroup1 += packageList.at(k).group1_List.at(i);
                                    }
                                locationC = sixteen2ten( tempgroup1 );
                                }
                            }
                            symtab.at(i).addr = ten2sixteen ( locationC );
                            symtab.at(i).addr = ( givezero( symtab.at(i).addr , 4 ) += symtab.at(i).addr ) ;
                        }
                        else {
                            symtab.at(i).addr = ten2sixteen ( locationC );
                            symtab.at(i).addr = ( givezero( symtab.at(i).addr , 4 ) += symtab.at(i).addr ) ;
                        }
                        existsym = true;
                    }

                }

                if ( existsym != true ) {
                    if ( locationC == 0 ) {                                                  // not start
                        if ( packageList.at(k).ins == "START" ) {
                            for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                                tempgroup1 += packageList.at(k).group1_List.at(i);
                            }
                            locationC = sixteen2ten( tempgroup1 );
                        }
                    }

                    tempsym.addr = ten2sixteen ( locationC );
                    tempsym.addr = ( givezero( tempsym.addr, 4 ) + tempsym.addr );
                    tempsym.word = packageList.at(k).label ;
                    symtab.push_back( tempsym ) ;
                    tempsym.word = "\0" ;
                    tempsym.addr = "\0" ;
                }
                existsym = false;
                tempgroup1 = "\0";
                //================================================================

                if( packageList.at(k).ins != "\0" ) {
                    if ( packageList.at(k).ins == "START" ) {
                        start = true;
                    }
                    else if ( packageList.at(k).ins == "END" ){ }
                    else if ( packageList.at(k).ins == "BYTE" ){ }
                    else if ( packageList.at(k).ins == "WORD" ){ }
                    else if ( packageList.at(k).ins == "RESB") { }
                    else if ( packageList.at(k).ins == "RESW" ){ }
                    else if ( packageList.at(k).ins == "EQU" ){}
                    else if ( packageList.at(k).ins == "BASE" ){}
                    else if ( packageList.at(k).ins == "LTORG" ){}
                    else  {
                        tempobj.objcode = findopcode ( packageList.at(k).ins , format );
                    }
                }

                if ( packageList.at(k).group1_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                        tempgroup1 += packageList.at(k).group1_List.at(i);
                    }
                    if ( packageList.at(k).ins == "START" ) {
                        locationC = sixteen2ten( tempgroup1 );
                    }

                    if ( packageList.at(k).ins == "WORD" ){                // WORD
                        tempobj.objcode = givezero( ten2sixteen( stoi( tempgroup1) ), 6 );
                        tempobj.objcode += ten2sixteen( stoi( tempgroup1  ) ) ;
                    }

                    else if( packageList.at(k).ins == "BYTE" ) {
                        int asc = 0;
                        if ( packageList.at(k).g1table == 6 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    tempobj.objcode += tempgroup1[i]  ;
                                }
                            }
                        }

                        else if ( packageList.at(k).g1table == 7 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    asc = (int)tempgroup1[i] ;
                                    tempobj.objcode += ten2sixteen( asc ) ;
                                    asc = 0;
                                }
                            }
                        }
                    }

                    else if ( packageList.at(k).ins == "EQU" ) {
                        int asc = 0 ;
                        asc = stoi( tempgroup1 );
                        locationC = sixteen2ten ( ten2sixteen( asc ) );
                    }

                    else if ( packageList.at(k).ins == "BASE" ) {
                        for (int i = 0; i < symtab.size() ; i ++ ) {
                            if ( tempgroup1 == symtab.at(i).word ) {
                                pc = sixteen2ten ( symtab.at(i).addr );
                            }

                        }
                    }

                    else {                                          // not start
                    }
                }
                if ( packageList.at(k).group2_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group2_List.size() ; i ++ ){
                            tempgroup2 += packageList.at(k).group2_List.at(i);
                    }

                    if ( tempgroup2 == "X" ) {}
                    else { }

                }
            }

            //====================================================================================
            else {                                             // ins
                if( packageList.at(k).ins != "\0" ) {
                    if ( packageList.at(k).ins == "START" ) {
                        start = true;
                    }
                    else if ( packageList.at(k).ins == "END" ) { }
                    else if ( packageList.at(k).ins == "BYTE" ) { }
                    else if ( packageList.at(k).ins == "WORD" ) { }
                    else if ( packageList.at(k).ins == "RESB")  { }
                    else if ( packageList.at(k).ins == "RESW" ) { }
                    else if ( packageList.at(k).ins == "EQU" )  { }
                    else if ( packageList.at(k).ins == "BASE" ) {}
                    else if ( packageList.at(k).ins == "LTORG" ){}
                    else if (packageList.at(k).ins == "RSUB" )   {
                        tempobj.objcode = findopcode ( packageList.at(k).ins ,format);
                        tempobj.objcode  += givezero(  tempobj.objcode , 6 );
                    }
                    else  {
                        tempobj.objcode = findopcode ( packageList.at(k).ins, format );
                    }
                }

                if ( packageList.at(k).group1_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){               // 先合併
                            tempgroup1 += packageList.at(k).group1_List.at(i);
                    }
                    if ( packageList.at(k).ins == "START" ) {
                        locationC = sixteen2ten( tempgroup1 );
                    }
                    if ( packageList.at(k).ins == "WORD" ){                                         // WORD
                        tempobj.objcode = givezero( ten2sixteen( stoi( tempgroup1) ), 6 );
                        tempobj.objcode += ten2sixteen( stoi( tempgroup1  ) ) ;
                    }

                    else if( packageList.at(k).ins == "BYTE" ) {
                        int asc = 0 ;
                        if ( packageList.at(k).g1table == 6 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    tempobj.objcode += tempgroup1[i]  ;
                                }
                            }
                        }
                        else if ( packageList.at(k).g1table == 7 ) {
                            for (int i = 0 ; i < tempgroup1.size() ; i ++ ) {
                                if ( tempgroup1[i] != '\'' ) {
                                    asc = (int)tempgroup1[i] ;
                                    tempobj.objcode += ten2sixteen( asc ) ;
                                    asc = 0;
                                }
                            }
                        }
                    }

                    else if ( packageList.at(k).ins == "EQU" ) {
                        int asc = 0 ;
                        asc = stoi( tempgroup1 );
                        locationC = sixteen2ten ( ten2sixteen( asc ) );
                    }
                    else if ( packageList.at(k).ins == "BASE" ) {
                        for (int i = 0; i < symtab.size() ; i ++ ) {
                            if ( tempgroup1 == symtab.at(i).word ) {
                                pc = sixteen2ten ( symtab.at(i).addr );
                            }

                        }
                    }

                    else { }
                }
                if ( packageList.at(k).group2_List.size() != 0 ) {
                    for(int i = 0; i < packageList.at(k).group2_List.size() ; i ++ ) {
                        tempgroup2 += packageList.at(k).group2_List.at(i);
                    }
                }
            }
            if ( packageList.at(k).format == 4 ) {
                format= 4;
            }
            tempobj.format = format;
            tempobj.tokenline = packageList.at(k).aline ;
            tempobj.loc = ten2sixteen( locationC );                     //================================
            tempobj.loc = ( givezero( ten2sixteen( locationC ) , 4 ) + tempobj.loc ) ;

            objlist.push_back( tempobj ) ;
            //=========================================================================================== locationC
            if ( packageList.at(k).ins == "BYTE" ) {
                if (packageList.at(k).g1table == 6 )  {                     // 'int'
                    locationC = locationC +  (packageList.at(k).group1_List.size() / 2 )  ;
                }

                else if (packageList.at(k).g1table == 7 )  {   //"string"
                    locationC = locationC  +  ( packageList.at(k).group1_List.size() )  ;
                }
            }
            else if ( packageList.at(k).ins == "RESB" ) {
                int i = stoi( tempgroup1 ) ;
                locationC = locationC + i ;
            }
            else if ( packageList.at(k).ins == "START" ) {}
            else if ( packageList.at(k).ins == "END" ) {}
            else if ( tempgroup1 == "." ) { }

            else {
                if ( objlist.at(k).format == 1 ) {
                    locationC = locationC + 1;
                }
                else if ( objlist.at(k).format == 2 ) {
                    locationC = locationC + 2;
                }
                else if ( objlist.at(k).format == 3 ) {
                    locationC = locationC + 3;
                }
                else if ( objlist.at(k).format == 4 ) {
                    locationC = locationC + 4;
                }
                else {
                    locationC = locationC + 3;
                }

            }

            tempgroup1 = "\0" ;
            tempgroup2 = "\0" ;
            tempobj.loc = "\0" ;
            tempobj.objcode = "\0" ;
            tempobj.tokenline = "\0" ;
            tempobj.format = 0 ;
            format = 0 ;
        }


        if ( start == false )   {
            cout << false;
        }

        for( int k = 0; k< packageList.size() ; k++ ) {                     //pass 2
            string tempOpjcode = "\0";
            string tempbin = "\0" ;
            string temp;
            string e = "0";                                                         // format 3 = 0 ; format 4 = 1;
            string b = "0";                                                         // use base register
            string p = "0";                                                         //  program counter
            string x = "0" ;                                                         //   index agister
            string i = "0";                                                         //   #  immediate mode
            string n = "0";                                                         //    @  indirect mode


            if ( objlist.at(k).format == 1 ) {                                  // ===========================format 1
                if ( packageList.at(k).group1_List.size() > 0 || packageList.at(k).group2_List.size() > 0 ) {
                    objlist.at(k).error = "error type ins  1";
                }
            }
            else if ( objlist.at(k).format == 2 ) {                                         // ===========================format 2

                if ( packageList.at(k).group2_List.size() == 0 ) {
                    tempOpjcode =  ten2sixteen( packageList.at(k).g1addr - 1 ) + "0" ;
                    objlist.at(k).objcode = objlist.at(k).objcode + tempOpjcode;
                }
                else {
                    tempOpjcode =  ten2sixteen( packageList.at(k).g1addr - 1 ) + ten2sixteen( packageList.at(k).g2addr  - 1 );
                    objlist.at(k).objcode = objlist.at(k).objcode + tempOpjcode;
                }

            }
            else if ( objlist.at(k).format == 3 ) {                                 // ===========================format 3
                if ( packageList.at(k).group2_List.size() > 0 ) {
                    objlist.at(k).error = "error type ins ";
                }

                else {
                    if ( packageList.at(k).group1_List.size() != 0 ) {
                        for( int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                            tempgroup1 += packageList.at(k).group1_List.at(i);
                        }
                    }
                    bool issym = false;
                    e = "0";
                    if ( packageList.at(k).group1_List.size() > 0 ) {
                        if ( packageList.at(k).immed == "1" ) {
                            n = "1" ;
                            i = "0" ;
                        }
                        else if ( packageList.at(k).indirect == "1" )   {
                            n = "0" ;
                            i = "1" ;
                        }
                        else {
                            n = "1" ;
                            i = "1" ;
                        }
                    }
                    else {
                        n = "1" ;
                        i = "1" ;
                    }

                    if ( packageList.at(k).group1_List.size() != 0 ) {
                        if ( tempgroup1 == "X" ) {
                            x = "1";
                        }
                        else {
                            x = "0";
                        }
                    }
                    else {
                        x = "0";
                    }

                    if ( ( sixteen2ten( objlist.at(k).loc) + 3 ) < 4096 ) {
                        b = "0";
                        p = "1";
                    }
                    else if ( pc < 4096 ){
                        b = "1";
                        p = "0";
                    }
                    else {
                       b = "1";
                       p = "0";
                    }


                    for( int i = 0; i < symtab.size() ; i++ ) {
                        if ( tempgroup1  == symtab.at(i).word ) {
                            tempOpjcode +=  symtab.at(i).addr ;
                            issym = true ;
                        }
                    }
                    if ( issym == false ) {
                        tempOpjcode += ten2sixteen ( stoi( tempgroup1 ) );
                    }



                    tempbin += hex2bin( objlist.at(k).objcode );
                    tempbin = tempbin.substr( 0 , 6 ) ;

                    if (issym == false ) {
                        temp = ( givezero ( tempOpjcode , 3)  +  tempOpjcode ) ;
                        p = "0";
                        b = "0";
                    }
                    else if  ( ( sixteen2ten( objlist.at(k).loc) + 3 ) < 4096 )  {
                        temp = ten2sixteen(  ( sixteen2ten( tempOpjcode ) - ( sixteen2ten(objlist.at(k).loc) + 3 ) )  ) ;              //addr - pc
                    }
                    else {
                        temp = ten2sixteen ( sixteen2ten( tempOpjcode ) - pc  );
                    }

                    tempbin += n;
                    tempbin += i;
                    tempbin += x;
                    tempbin += b;
                    tempbin += p;
                    tempbin += e;

                    if ( temp.size() > 3 ) {
                        temp = temp.substr( ( temp.size() - 3) , 3 );
                    }

                    tempbin += hex2bin  ( temp );
                    objlist.at(k).objcode = ten2sixteen( bin2ten( tempbin )  ) ;
                }
            }

            else if ( objlist.at(k).format == 4 ) {                         // ===========================format 4
                if ( packageList.at(k).group2_List.size() > 0 ) {
                    objlist.at(k).error = "error type ins 4";
                }
                else {
                    if ( packageList.at(k).group1_List.size() != 0 ) {
                        for(int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                            tempgroup1 += packageList.at(k).group1_List.at(i);
                        }
                    }
                    b = "0";
                    p = "0";
                    e = "1";
                    if ( packageList.at(k).group1_List.size() > 0 ) {
                        if ( packageList.at(k).immed == "1" ) {
                            n = "1" ;
                            i = "0" ;
                        }
                        else if ( packageList.at(k).indirect == "1" )   {
                            n = "0" ;
                            i = "1" ;
                        }
                        else {
                            n = "1" ;
                            i = "1" ;
                        }
                    }
                    else {
                        n = "1" ;
                        i = "1" ;
                    }

                    if ( packageList.at(k).group1_List.size() != 0 ) {
                        if ( tempgroup1 == "X" ) {
                            x = "1";
                        }
                        else {
                            x = "0";
                        }
                    }
                    else {
                        x = "0";

                    }

                    tempbin += hex2bin( objlist.at(k).objcode );
                    tempbin = tempbin.substr( 0 , 6 ) ;
                    tempbin += n;
                    tempbin += i;
                    tempbin += x;
                    tempbin += b;
                    tempbin += p;
                    tempbin += e;

                    if ( packageList.at(k).group1_List.size() != 0 ) {
                        if ( packageList.at(k).group2_List.size() == 0 )  {
                            for( int i = 0; i < symtab.size() ; i++ ) {
                                if ( tempgroup1  == symtab.at(i).word ) {
                                    //tempOpjcode += symtab.at(i).addr;
                                    tempbin += "0000";
                                    tempbin +=  hex2bin( symtab.at(i).addr );
                                }
                            }
                        }
                    }

                    objlist.at(k).objcode = ten2sixteen( bin2ten( tempbin )  ) ;
                    //cout << ten2sixteen( bin2ten( tempbin )  ) << endl;

                }
            }
            else {
                for( int i = 0; i < packageList.at(k).group1_List.size() ; i ++ ){
                tempgroup1 += packageList.at(k).group1_List.at(i);
                }
                if ( packageList.at(k).group1_List.size() != 0 ) {
                    if ( packageList.at(k).group2_List.size() == 0 )  {
                        for( int i = 0; i < symtab.size() ; i++ ) {
                            if ( tempgroup1  == symtab.at(i).word ) {
                                objlist.at(k).objcode  += symtab.at(i).addr;
                            }
                        }
                    }
                }

            }

            //========================================================================================================

            if ( packageList.at(k).ins == "END" ) {
                cout << setw(3) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else if ( objlist.at(k).error != "\0" ) {
                cout << objlist.at(k).error << endl;
            }
            else {
                cout << setw(3) << 5*k + 5 << "\t"<< bigword ( objlist.at(k).loc )  << "\t" << objlist.at(k).tokenline << "\t\t\t" << bigword ( objlist.at(k).objcode )  <<endl  ;
            }
            tempgroup1 = "\0";
            tempgroup2 = "\0";
            e = "0";
            b = "0";
            p = "0";
            x = "0";
            i = "0";
            n = "0";
            tempbin = "\0";
            tempOpjcode = "\0";
            temp = "\0";

        }


    }



    void outfile3() {

        ofstream out ;
        out.open( "my_sicxe_output.txt" );
        out << "Line  Location  Source code                              Object code" << endl ;
        out << "----  -------- -------------------------                 -----------" << endl;
        for( int k = 0; k< packageList.size() ; k++ ) {
            if ( packageList.at(k).ins == "END" ) {
                out << setw(4) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else if (packageList.at(k).trash != "\0") {
                out << setw(4) << 5*k + 5 << "\t"<< "\t" << objlist.at(k).tokenline << "\t\t\t"   <<endl  ;
            }
            else if ( objlist.at(k).error != "\0" ) {
                out << objlist.at(k).error << endl;
            }
            else {
                out << setw(4) << 5*k + 5 << "\t"<< bigword ( objlist.at(k).loc )  << "\t" << objlist.at(k).tokenline << "\t\t\t" << bigword ( objlist.at(k).objcode )  <<endl  ;
            }
        }
        out.close();
    }

    void clearall() {
        wordlist.clear();
        tablelist.clear();
        packageList.clear();
        symtab.clear();
        objlist.clear();
    }
};



Token token1;
bool getfile( int mission ) {
        string cin_dataname  ;
        string copyname  ;
        int num;
        do
        {
           switch ( mission )
           {
                case 1: {
                    copyname = "\0" ;
                    cin_dataname = "\0";
                    cout << "輸入Table號碼, ...[0]Quit):" ;
                    cin >> cin_dataname ;
                    if( cin_dataname == "0" )               // 如果輸入0，代表不執行此任務，並回傳false
                        return false ;
                    copyname = cin_dataname ;               // 目標紀錄輸入的數字
                    cin_dataname =  "Table" + cin_dataname + ".table" ;
                    fs.open( cin_dataname ) ;
                    if( fs.is_open() == false ) 	                        // 先檢測是否存在，沒有的話要輸出錯誤資訊
                        cout << endl << "### " + cin_dataname + " does not exist! ###" << endl << endl ;
                    break;
                }

                case 2: {
                    copyname = "\0" ;
                    cin_dataname = "\0";
                    cout << "輸入檔名 ex: SIC_input ...[0]Quit):" ;
                    cin >> cin_dataname ;
                    if( cin_dataname == "0" )               // 如果輸入0，代表不執行此任務，並回傳false
                        return false ;
                    copyname = cin_dataname ;                // 目標紀錄輸入的數字
                    cin_dataname =  cin_dataname + ".txt" ;
                    fs.open( cin_dataname ) ;
                    if( fs.is_open() == false ) 	                        // 先檢測是否存在，沒有的話要輸出錯誤資訊
                        cout << endl << "### " + cin_dataname + " does not exist! ###" << endl << endl ;
                    break;
                }

                case 3: {
                    copyname = "\0" ;
                    cin_dataname = "\0";
                    cout << "輸入檔名 ex: SIC_input ...[0]Quit):" ;
                    cin >> cin_dataname ;
                    if( cin_dataname == "0" )               // 如果輸入0，代表不執行此任務，並回傳false
                        return false ;
                    copyname = cin_dataname ;               // 目標紀錄輸入的數字
                    cin_dataname =  cin_dataname + ".txt" ;
                    fs.open( cin_dataname ) ;
                    if( fs.is_open() == false ) 	                        // 先檢測是否存在，沒有的話要輸出錯誤資訊
                        cout << endl << "### " + cin_dataname + " does not exist! ###" << endl << endl ;
                    break;
                }
                case 4: {
                    copyname = "\0" ;
                    cin_dataname = "\0";
                    cout << "輸入檔名 ex: SICXE_input ...[0]Quit):" ;
                    cin >> cin_dataname ;
                    if( cin_dataname == "0" )               // 如果輸入0，代表不執行此任務，並回傳false
                        return false ;
                    copyname = cin_dataname ;               // 目標紀錄輸入的數字
                    cin_dataname =  cin_dataname + ".txt" ;
                    fs.open( cin_dataname ) ;
                    if( fs.is_open() == false ) 	                        // 先檢測是否存在，沒有的話要輸出錯誤資訊
                        cout << endl << "### " + cin_dataname + " does not exist! ###" << endl << endl ;
                    break;
                }

            }// end switch
        }
        while( fs.is_open( ) != true ) ;
            switch ( mission )    // 選擇任務一或二
                {
                    case 1:  {
                        num = stoi(copyname);
                        token1.gettable( num );
                        //token1.show_ins();
                        num = 0;
                        fs.close();
                        break;
                     }
                    case 2:  {
                        token1.gettoken();
                        token1.cuttoken();
                        token1.showtoken();
                        token1.outfile();
                        token1.clearall();
                        token1.prepare();
                        fs.close();
                        break;
                     }
                    case 3:  {
                        token1.gettoken();
                        token1.cuttoken();
                        token1.makegroup();
                        token1.showallgroup();
                        token1.makesic();
                        token1.outfile2();
                        token1.clearall();
                        token1.prepare();
                        fs.close();
                        break;
                     }
                    case 4:  {
                        token1.gettoken();
                        token1.cuttoken();
                        token1.makegroup();
                        token1.showallgroup();
                        token1.makesicxe();
                        token1.outfile3();
                        token1.clearall();
                        token1.prepare();
                        fs.close();
                        break;
                     }


                } // end switch
}

int main()
{
    int command = 0; // user command
    token1.prepare();
	do
	{
    	cout << endl << "***************************************";
    	cout << endl << " 0. QUIT                              *";
    	cout << endl << " 1: open table                        *";
    	cout << endl << " 2: Lexical Analysis                  *";
    	cout << endl << " 3: SIC                               *";
    	cout << endl << " 4: SICXE                             *";
    	cout << endl << "***************************************";
    	cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
    	cin >> command; // get a command
    	switch (command)
    	{
        	case 0: {
                break;
        	}
         	case 1: {
                    getfile(1) ;
                	break;
         	}
        	case 2: {
                    getfile(2) ;
					break ;
            }
            case 3: {
                    getfile(3) ;
					break ;
            }
            case 4: {
                    getfile(4) ;
					break ;
            }
        	default: cout << endl << "Command does not exist!" << endl;
    	} // end switch
	}
	while ( command != 0); // '0': stop the program
    	system("pause"); // pause the display
    return 0;
}

//plagarismCatcher

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "HashMap.h"
using namespace std;

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    for(vector<string>::iterator i=files.begin();i!=files.end();i++){
        if(*i==".." || *i==".") {
            files.erase(i);
            i--;
        }
    }

    return 0;
}

string minifyChunk(vector<string> rawChunk) {
    string chunk="";
    for(vector<string>::iterator i=rawChunk.begin();i!=rawChunk.end();++i) {
        string word=*i;
        for(string::iterator j=word.begin();j!=word.end();++j) {
            int asciiVal=(int)*j;
            if(asciiVal>64 && asciiVal<91) {//capital letter
                asciiVal+=32;
                *j=asciiVal;
            }
            else if(asciiVal<97 || asciiVal >122) {//punctuation
                word.erase(j);
                j--;
            }
        }
        chunk+=word;
    }
    return chunk;
}
void chunk(string dir,vector<string> &files,int n,HashMap &h) {
    //cout<<"start"<<endl;

    //autocomplete dir
    if(dir[dir.size()-1] != '/') {
        dir+="/";
    }

    for(vector<string>::iterator i=files.begin();i!=files.end();++i) {
        ifstream infile;
        infile.open(dir+(*i),ios::in);
        vector<string> words;
        string temp="";
        while(infile>>temp) {words.push_back(temp);}
        //cout<<"words size: "<<words.size()<<endl;



        for(vector<string>::iterator j=words.begin();j!=words.end()-n;++j) {

            vector<string> rawChunk;
            for(int k=0;k<n;k++) {
                rawChunk.push_back(*(j+k));
            }
            h.insert(minifyChunk(rawChunk),*i);

        }
        //get first word
        
        words.clear();
        infile.close();
    }
}

int main(int argc,char* argv [])
{
    if(argc<3) {
        cout<<"Error: please use syntax ./plagiarismCatcher [path/to/files] [chunk size]"<<endl;
        return -1;
    }
    string dir = argv[1];
    vector<string> files = vector<string>();
    getdir(dir,files);
   
    // for (unsigned int i = 0;i < files.size();i++) {
    //     cout << i << files[i] << endl;
    // }
   
   /*
   ifstream test;
   test.open("sm_doc_set/"+files.front(),ios::in);
   string s;
    while(test>>s) {
   cout<<s<<endl; }
   
   */
   
   HashMap h;
   //h.insert("testonetwothreefourfiv3","file");
   //cout<<h.search("testonetwothreefodurfiv3","file");
   
    int n=atoi(argv[2]);
    chunk(dir,files,n,h);
    h.getCollisions(files);
    return 0;

}

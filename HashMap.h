//string HashMap

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

class HashMap {
    const static int m = 128189;
    public:

        struct node {
            node * next;
            string fileName;
        };
        //typedef struct node node_t;

        HashMap() {
            for(int i =0;i<m;i++) {
                map[i]=NULL;
            }
        }
        void insert(string s,string file) {
            int index=hash(s);
            //cout<<s<<" hashed into "<<index<<endl;
            if(map[index]==NULL) {
                node *tmp=new node();
                tmp->fileName=file;
                map[index]=tmp;
            }
            else {
                //cout<<"collision with " <<s<<" in "<<file<<" and "<<map[index]->fileName<<endl;
                node *tmp=new node();
                tmp->fileName=file;
                tmp->next=map[index];
                map[index]=tmp;
            }
        }

        node* search(string s,string file) {
            int index=hash(s);
            node *p = map[index];
            while(p!=NULL) {
                if(p->fileName==file)
                    return p;
                p=p->next;
            }
            return NULL;
        }
        int getIndex(string fileName, vector<string> files) {
            for(int i =0;i<files.size();i++) {
                if(fileName==files[i]) {
                    return i;
                }
            }
            return -1;
        }
        void getCollisions(vector<string> files) {
            int collisions[files.size()][files.size()];
            for(int i =0;i<files.size();i++) {
                for(int j=0;j<files.size();j++) {
                    collisions[i][j]=0;
                }
            }
            for(int i =0;i<m;i++) {
                node* p=map[i];
                while(p!=NULL) {
                    int wrkIndex=getIndex(p->fileName,files);
                    node * q=p->next;
                    while(q!=NULL) {
                        int second=getIndex(q->fileName,files);
                        //cout<<wrkIndex<< " x "<<second<<endl;
                        if(second!=wrkIndex) { //upper triangle, add to array
                            //cout<<"adding " <<wrkIndex<< ", "<<second<<endl;
                            collisions[wrkIndex][second]++;
                        }
                        q=q->next;
                    }
                    p=p->next;
                }
                
            }
            vector<string> outputs;
            for(int i =0;i<files.size();i++) {

                for(int j=0;j<i;j++) {
                    
                    int col=collisions[i][j];
                    if(col>=200&&col<1000) {
                        string out=" "+std::to_string(static_cast<long long>(col))+ " : "+files[j]+", "+files[i];
                        outputs.push_back(out);
                    }
                    else if (col>=200) {
                        string out=std::to_string(static_cast<long long>(col))+ " : "+files[j]+", "+files[i];
                        outputs.push_back(out);
                    }
                }
            } 
            sort(outputs.begin(),outputs.end());
            for(int i =outputs.size()-1;i>=0;i--) {
                cout<<outputs[i]<<endl;
            }
            /*
            for(int i =0;i<files.size()-1;i++) {
                for(int j=0;j<files.size();j++) {
                    cout<<collisions[i][j]<<"   ";
                }
                cout<<endl;
            }
            */
        }
        ~HashMap() {
            for(int i=0;i<m;i++) {
                node *p = map[i];
                node *prev=NULL;
                while(p!=NULL) {
                    delete prev;
                    prev=p;
                    p=p->next;
                }
            }
        }
    private:
        node *map[m];
        int hash(string chunk) {
            int code=0;
            for(int i=0;i<chunk.size();i++) {
                int p=1;
                for(int j=0;j<chunk.size()-i;j++) {
                    p*=31;
                }
                code+=p*chunk[i];            
            }
            if(code<0) {code*=-1;}
            return code%m;
        }
        
        

};

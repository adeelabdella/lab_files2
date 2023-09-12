#include<iostream>
#include<conio.h>
#include<cstring>
#include<map>
#include<set>

using namespace std;



class Sentence{
      string sentence;
      map < int, string > indWord;
      set < string > visited;
      int index;
      void hashSentence();
      bool check(char x);
      public:
             string getReverse();
             void set();  
             string get();
             string getRemoved();  
};
string Sentence::getReverse(){
       string result = "";
       if(visited.size() == 0)hashSentence();
       for(int i = 0; i < index; i++){
           result += string(indWord[i].rbegin(), indWord[i].rend());
           result += " ";
       }
       return result;
}
bool Sentence::check(char x){
     return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z');
}
void Sentence::set(){
                cout<<"Enter your sentence"<<endl;
                cin >> ws;
                getline(cin, sentence,'.');
                sentence.push_back(' ');
}

string Sentence::get(){
       return sentence;
}

void Sentence::hashSentence(){
     string s = "";
     index = 0;
     for(int i = 0; i < sentence.size(); i++){
             if(!check(sentence[i])){
                 if(visited.find(s) == visited.end() && !s.empty()){
                     visited.insert(s);
                     indWord[index] = s;
                     index++;
                 }
                 s.clear();
                 continue;
             }
             s.push_back(sentence[i]);
     }
}

string Sentence::getRemoved(){
       string result = "";
       hashSentence();
       for(int i = 0; i < index; i++){
           result += indWord[i];
           result += " ";
       }
       return result;
}

int main(){
    Sentence obj;
    obj.set();
    cout<<obj.getRemoved()<<endl;
    cout<<obj.getReverse()<<endl;
    getch();
    return 0;
}

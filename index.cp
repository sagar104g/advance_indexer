#include <bits/stdc++.h>
#include "index.h"

//function use to open file where old data saved by indexer sotored and then lode it into program
supermap::supermap(){
    file.open("/home/vipin/Project/file/index.txt");
    loadfromfile();
};

//this function load data from file to data structure
void supermap::loadfromfile(){
    std::string line;
    for ( ;getline(file, line);) {          //here loop will end when all lines will visited
        std::string word="";
        int i=0;
        while(line[i]!=' ')                 //this loops runs till when we found space in words
            word+=line[i++];
        i++;
        while(line[i]!='\0'){               //this loop will end after end of line
            std::string nn="";
            while(line[i]!=' ')
                nn+=line[i++];
            i++;
            int occur=std::stoi(nn,0,10);   //it convert string to int
            nn="";
            while(line[i]!=' ')
                nn+=line[i++];
            insert(word,occur,nn);          //it will insert the old indxed data from file to data structure
            i++;
        }
    }
    file.close();
}

//this function will save indexed data to file for future use
void supermap::savetofile(){
    std::ofstream nfile;
    nfile.open("/home/vipin/Project/file/index.txt");                                //it will open the file in which we want to save data
    std::set<std::pair<int,std::string> >::iterator itr1;
    std::unordered_map<std::string,std::set<std::pair<int,std::string> > >::iterator itr2;
    std::unordered_map<char,std::unordered_map<std::string,std::set<std::pair<int,std::string> > > >::iterator itr3;
    for(itr3=mainmap.begin(); itr3!=mainmap.end(); itr3++)
        {
        for(itr2=itr3->second.begin(); itr2!=itr3->second.end(); itr2++)
            {
            nfile<<(*itr2).first<<" ";
            for(itr1=itr2->second.begin(); itr1!=itr2->second.end(); itr1++)
               nfile<<(*itr1).first<<" "<<(*itr1).second<<" ";                      //this will put data in file
            nfile<<std::endl;
            }
        }
    nfile.close();   
}

//it will insert data in main map where all data has been stored (from file)
void supermap::insert(std::string word, int occurance, std::string url){
    mainmap[word[0]][word].insert(std::pair<int,std::string>(occurance,url));
}

//it will insert data in main map where all data has been stored (when program is runing)
void supermap::insert(std::unordered_map<std::string,int> wordmap ,std::string url)
{
    std::unordered_map<std::string,int>::iterator it;
    for(it=wordmap.begin();it!=wordmap.end();it++)
        {
        insert((*it).first,(*it).second,url);
        }
}

//this function will use to open files which are crawled by crawler and get there page main url 
words::words(std::string& filename){
    file.open("/home/vipin/Project/crawlerdir/"+filename);
    url = getUrlFromFile(file);
    if(!file.is_open())                                      //if file does'nt exist this if condition will work
        filename="NAN";
} 


//get url from files from where they are crawled
std::string words::getUrlFromFile(std::ifstream& file){
    std::string line;
    getline(file, line);
    return line;
}


//this function will make a mini map
void words::makeMiniMap(std::string entry, int entrysize, std::string url,supermap& index)
{
	if(entrysize!=-1)
	{
	   std::transform(entry.begin(), entry.end(), entry.begin(), ::tolower);   //it will convert whole string to lowercase
	   wordmap[entry]=wordmap[entry]+entrysize;
	}
    else
    {
        index.insert(wordmap,this->url);
    }
}

//load file data to string
std::string words::toBuffer() {
    file.seekg(0,std::ios::end);
    int len=file.tellg();
    std::string s;
    file.seekg(0,std::ios::beg);
    int i=0;
    while(!file.eof()) {
        char temp;
        file>>std::noskipws>>temp; //add all data of file to string
        s+=temp;
    }
    return s;
}

// this function is use to get words from al pages(data from meta,tile and heading tags)
void words::getWord(supermap& index)
{
	std::string buffer=toBuffer();
	size_t pos,endPos,i=0,endp=buffer.length();
	
/*-----------------------------------------<title>--------------------------------------------------------------------*/	

	pos=buffer.find("<head>");
	endPos=buffer.find("</head>");
	pos=buffer.find("<title>",pos);
	pos+=7;
	if(pos<endPos){	
		std::string str;     
		int flag=0;
		while(buffer[pos]!='<'){   
			if(buffer[pos]>0 && buffer[pos]<=127 && buffer[pos]!=0 && buffer[pos]!=10 && buffer[pos]!='\r' && buffer[pos]!='\t' && buffer[pos]!='\v' && buffer[pos]!='\b' && buffer[pos]!='\f' && buffer[pos]!='\a')
				str+=buffer[pos];
			pos++;
		}
		char *temp=strtok(&str[0]," ,.:|/+-;!@#$^&*?()"); /* \n\r\t\v\b\f\a */
		while(temp!=NULL){
			makeMiniMap(temp,3,this->url,index);
			temp=strtok(NULL," ,.:|/+-;!@#$^&*?()");
		}
	}

/*----------------------------------------<meta>-------------------------------------------------------------------*/

	pos=buffer.find("<head>");
	size_t pos2;
	while(pos<endPos){
		pos=buffer.find("<meta",pos);
        if(pos>endp)
            break;
        pos2=buffer.find(">",pos);
        if(buffer.find("description",pos)<pos2||buffer.find("keywords",pos)<pos2||buffer.find("Description",pos)<pos2||buffer.find("Keywords",pos)<pos2)
        {
            pos=buffer.find("content",pos);
            pos+=9;
            std::string str;
            int flag=0;
			while(buffer[pos]!='"'){
				if(buffer[pos]>0 && buffer[pos]<=127 && buffer[pos]!=0 && buffer[pos]!=10 && buffer[pos]!='\r' && buffer[pos]!='\t' && buffer[pos]!='\v' && buffer[pos]!='\b' && buffer[pos]!='\f' && buffer[pos]!='\a')
				str+=buffer[pos];
				pos++;
			}
			char *temp=strtok(&str[0]," ,.:|/+-;!@#$^&*?()"); /* \n\r\t\v\b\f\a */
			while(temp!=NULL){
				makeMiniMap(temp,2,this->url,index);
				temp=strtok(NULL," ,.:|/+-;!@#$^&*?()");
			}
        }
        pos=pos2;
    }

/*-------------------------------------------------<h>---------------------------------------------------------------*/

    pos=buffer.find("<body");
    while(pos!=0){
    	pos=buffer.find("<h",pos);
    	if(buffer[pos+2]=='1' || buffer[pos+2]=='2' || buffer[pos+2]=='3' || buffer[pos+2]=='4' || buffer[pos+2]=='5' || buffer[pos+2]=='6'){
    		pos=buffer.find(">",pos);
    		size_t endline=buffer.find("</h",pos);
    		pos++;
    		while(pos<endline){
    			int flag=0;
    			std::string str;
    			if(buffer[pos]=='<' && pos<endline){
    				while(buffer[pos]!='>')
    					pos++;
    				pos++;
    			}
    			while(buffer[pos]!='<' && pos<endline){
    				flag=1;
					if(buffer[pos]>0 && buffer[pos]<=127 && buffer[pos]!=0 && buffer[pos]!=10 && buffer[pos]!='\r' && buffer[pos]!='\t' && buffer[pos]!='\v' && buffer[pos]!='\b' && buffer[pos]!='\f' && buffer[pos]!='\a')
					str+=buffer[pos];
					pos++;
				}
				if(flag==1){
					//cout<<str<<endl;
					char *temp=strtok(&str[0]," ,.:|/+-;!@#$^&*?()"); /* \n\r\t\v\b\f\a */
					while(temp!=NULL){
						makeMiniMap(temp,1,this->url,index);
						temp=strtok(NULL," ,.:|/+-;!@#$^&*?()");
					}
				}	
			}
    	}
    	else
    	pos++;
   }
   makeMiniMap("",-1,"",index);
}

//this function use to convert int to string 
char* toString(int a)
{
    char* str=new char[5];
    sprintf(str,"%d",a);
    return str;
}



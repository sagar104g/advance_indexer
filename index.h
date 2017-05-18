#ifndef _INDEX_H_
#define _INDEX_H_

//super map class
class supermap{
    private:
        std::unordered_map<char,std::unordered_map<std::string,std::set<std::pair<int,std::string> > > > mainmap;
        std::ifstream file;
    public:
        supermap();                                                   //function use to open file where old data saved by indexer sotored and then lode it into program
        void loadfromfile();                                          //this function load data from file to data structure
        void savetofile();                                            //this function will save indexed data to file for future use
        void insert(std::unordered_map<std::string,int>,std::string); //it will insert data in main map where all data has been stored (from file)
        void insert(std::string,int,std::string);                     //it will insert data in main map where all data has been stored (when program is runing)
};

//words class
class words{
    private:
          std::unordered_map<std::string,int> wordmap;
          std::string url;
          std::ifstream file;
    public:
          words(std::string&);                                       //this function will use to open files which are crawled by crawler and get there page main url 
          std::string getUrlFromFile(std::ifstream&);                //get url from files from where they are crawled
          void makeMiniMap(std::string, int, std::string, supermap&);//this function will make a mini map
          void getWord(supermap&);                                   // this function is use to get words from al pages(data from meta,tile and heading tags)
          std::string toBuffer();                                    //load file data to string
          
};

char* toString(int a);                                               //this function use to convert int to string 

#endif
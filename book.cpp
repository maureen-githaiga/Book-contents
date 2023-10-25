/*
 * COMP.CS.110 Project 3: BOOK CONTENTS
 * This program reads the contents of a book chapter and its
 * subchapters.Takes their id,fullname,length and their parent chapters.
 *
 * Notes about the program and it's implementation:
 *
 * addNewChapter- creates a pointer to a struct containing information
 *  about a chapter.Takes the id,fullname,length and also whether also
 *   initilises the chapter as open.
 * addRelation- links the parent chapters and the subchapters
 * printIds - Prints fullnames of book chapters and their ids
 * printContents - Prints the book chapters in their order showing the parents
 *  and subchapters
 * close- Closes a given chapter
 *  open- opens a given chapter
 * printParentsN -prints the parents of a given chapter
 *printSubchaptersN -prints subchapters of a given chapter
 * printSiblingChapters -prints chapters sharing the same parent
 * printTotalLength - prints the total length of a chapter with its subchapters.
 *printLongestInHierarchy -prints the longest in hierachy checks the length
 *of a chapter.
 *
 *
 * Program author
 * Name: Maureen Githaiga
 * Student number: 151227842
 * UserID: vpmagi
 * E-Mail: githaigamaureen@tuni.fi
 * */
#include "book.hh"
#include <algorithm>


Book::Book()
{

}

Book::~Book()
{
    //deallocating memory
    for(auto& ch : chapters_)
    {
        delete ch.second;
        ch.second = nullptr;

    }

}

void Book::addNewChapter(const std::string &id, const std::string &fullName,
                         int length)
{
    //getting order of ids
    orders_.push_back(id);

    //if chapter doesn't exists create chapter pointer
    if(chapters_.find(id) == chapters_.end())
    {
        Chapter* new_chapter = new Chapter{id, fullName, length };
        chapters_.insert(std::make_pair(new_chapter->id_,new_chapter));
        return;

    }

    //if chapter exists
    std::cout << "Error: Already exists." << std::endl;

}

void Book::addRelation(const std::string &subchapter,
                       const std::string &parentChapter)
{
    //if chapter exists

    if(chapters_.find(subchapter) == chapters_.end()
            ||chapters_.find(parentChapter) == chapters_.end())
        return;

    //getting subchapter

    Chapter* sub_ptr = chapters_.at(subchapter),
            * parent_ptr = chapters_.at(parentChapter);

    //adding subchapter
    parent_ptr->subchapters_.push_back(sub_ptr);
    sub_ptr->parentChapter_= parent_ptr;

}

void Book::printIds(Params /*params*/) const
{
    //counting ids in chapters map
    Data::const_iterator iter1 = chapters_.begin();
    int count = 0;
    for(;iter1 != chapters_.end(); ++iter1)
    {
        count+=1;
    }
    std:: cout << "Book has " << count << " chapters:" << std::endl;

    //copying the fullname and id to map prints alphabetically by fullname
    std::map<std::string,std::string> ids = {};
    for(auto& it : chapters_)
    {
        ids.insert({it.second->fullName_,it.second->id_});
    }

    for(auto& elem : ids)
    {
        std:: cout << elem.first << " --- " << elem.second << std::endl;
    }


}

void Book::printContents(Params /*params*/) const
{

    //finding top most chapters
    std::vector<Chapter*> topmost;
    for(unsigned int i = 0; i < orders_.size(); i++)
    {

        for(auto& ch : chapters_)
        {

            if(orders_.at(i) == ch.first &&
                    ch.second->parentChapter_ == nullptr )
            {
                topmost.push_back(ch.second);

            }
        }

    }
    //printing chapters in topmost
    int counter = 1;
    for(Chapter* ch : topmost)
    {
        printChapterRecursive(ch, "", counter++);
    }


}

void Book::close(Params params) const
{
    std::string id = params.at(0);
    //if id not found
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }

    //closes the chapter
    for(std::pair<std::string, Chapter*> chapter : chapters_)
    {
        if(chapter.first== id)
        {
            chapter.second->is_open_ = false;

        }
    }

}

void Book::open(Params params) const
{
    std::string id = params.at(0);
    //if id not found
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }

    Chapter* ptr = findChapter(id);
    ptr->is_open_ = true;
    std::vector< Chapter*> subchapters = ptr->subchapters_;
    //opens chapters in subchapters
    for(auto& chapter : subchapters)
    {
        chapter->is_open_ = false;
        if(chapter->subchapters_.empty() == true)
        {
            chapter->is_open_= true;
            return;
        }


    }

}

void Book::openAll(Params /*params*/) const
{
    //opens the chapter in the struct
    for(std::pair<std::string, Chapter*> chapter : chapters_)
    {
        chapter.second->is_open_ = true;

    }


}

void Book::printParentsN(Params params) const
{
    std::string id = params.at(0);
    int level = stoi(params.at(1));
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }
    else if(level < 1)
    {
        std::cout << "Error. Level can't be less than 1." << std::endl;
        return;
    }

    int current_level = 1;
    std::vector<Chapter*> parents = {findChapter(id)};

    for(auto& chapter : parents)
    {
        //if chapter has no parent
        if(chapter->parentChapter_ == nullptr)
        {
            std::cout << id << " has no parent chapters." << std::endl;
            return;
        }
        //if it has a parent
        for(;;)
        {
            parents.push_back(parents.back()->parentChapter_);

            if(level == current_level ||
                    parents.back()->parentChapter_ == nullptr)
            {
                break;
            }
            current_level+=1;


        }

    }

    //erasing id since chapter cannot be a parent to itself
    parents.erase(parents.begin());
    //sorting alphabetically
    std::sort(parents.begin(),
              parents.end(),
              [](const auto &a,const auto &b){return a->id_ < b->id_;});

    if (parents.size() > 0)
    {
        std::cout << id << " has " << parents.size()
                  << " parent chapters:" << std::endl;
        for(auto& elem : parents)
        {
            std::cout << elem->id_ << std::endl;
        }
    }


}

void Book::printSubchaptersN(Params params) const
{
    std::string id = params.at(0);
    int level = stoi(params.at(1));
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }
    else if(level < 1)
    {
        std::cout << "Error. Level can't be less than 1." << std::endl;
        return;
    }
    IdSet subchapters;

    for(auto& ch: chapters_)
    {
        //getting subchapters recurssively
        if(ch.first == id)
        {

            printsubchaptersRecursive(ch.second->subchapters_,
                                      subchapters,level);
        }


    }
    if(subchapters.size() >= 1)
    {
        std::cout << id << " has " << subchapters.size()
                  << " subchapters:" << std::endl;
        for(auto it = subchapters.begin(); it != subchapters.end();it++)
        {
            std:: cout << *it << std::endl;

        }
    }
    else
    {
        std::cout << id << " has no subchapters." << std::endl;
    }





}

void Book::printSiblingChapters(Params params) const
{
    std::string id = params.at(0);
    IdSet siblings;
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }

    Chapter* ptr = findChapter(id);

    if(ptr->parentChapter_ == nullptr)
    {
        std::cout << id << " has no sibling chapters." << std::endl;
        return;

    }

    std::vector< Chapter* > subchapters = ptr->parentChapter_->subchapters_;
    for(auto& chapter : subchapters)
    {
        siblings.insert(chapter->id_);

    }
    siblings.erase(id);
    std::cout << id << " has " << siblings.size()
              << " sibling chapters:" << std::endl;
    for(auto it = siblings.begin(); it != siblings.end();it++)
    {
        std:: cout << *it << std::endl;

    }

}

void Book::printTotalLength(Params params) const
{
    std::string id = params.at(0);
    int total_length = 0;
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }
    std::vector<Chapter*> subchapters;
    for(auto& ch : chapters_)
    {

        if (ch.first == id)
        {
            subchapters.push_back(ch.second);
            getsubchaptersRecursive(ch.second->subchapters_,subchapters);
        }



    }
    std::vector<int>lengths;
    for(auto& chapter : subchapters)
    {
        lengths.push_back(chapter->length_);
    }


    for(std::vector<int>::iterator it = lengths.begin();
        it!=lengths.end();++it)
    {
        total_length +=*it;
    }
    std::cout << "Total length of " << id
              << " is " << total_length
              << "." << std::endl;



}

void Book::printLongestInHierarchy(Params params) const
{
    std::string id = params.at(0);
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }
    //container for chapters and their lengths
    std::vector<Chapter*> longests;

    for(auto& chapter : chapters_)
    {

        if (chapter.first == id)
        {
            longests.push_back(chapter.second);
            getsubchaptersRecursive(chapter.second->subchapters_,longests);
        }



    }
    Chapter* longest = nullptr;
    for(auto& element : longests)
    {
        if(longest == nullptr || longest->length_ < element->length_)
        {
            longest = element;
        }
    }
    if(longest == nullptr)
    {
        return;
    }
    //if the longest is as the given id
    if(longest->id_ == id)
    {
        std::cout << "With the length of " << longest->length_
                  << ", " << longest->id_
                  << " is the longest chapter in their hierarchy." << std::endl;
    }
    else
    {
        std::cout << "With the length of " << longest->length_
                  << ", " << longest->id_
                  << " is the longest chapter in " << id
                  << "'s hierarchy." << std::endl;
    }


}

void Book::printShortestInHierarchy(Params params) const
{
    std::string id = params.at(0);
    if(!findChapter(id))
    {
        std:: cout << "Error: Not found: " << id << std::endl;
        return;
    }
    //container for chapters and their lengths
    std::vector<Chapter*> shortests;

    for(auto& chapter : chapters_)
    {
        if (chapter.first == id)
        {
            shortests.push_back(chapter.second);
            getsubchaptersRecursive(chapter.second->subchapters_,shortests);
        }


    }
    Chapter* shortest = nullptr;
    for(auto& element : shortests)
    {
        if(shortest == nullptr || shortest->length_ > element->length_)
        {
            shortest = element;
        }
    }
    if(shortest == nullptr)
    {
        return;
    }
    //if the shortest is as the given id
    if(shortest->id_ == id)
    {
        std::cout << "With the length of " << shortest->length_
                  << ", " << shortest->id_
                  << " is the shortest chapter in their hierarchy."
                  << std::endl;
    }
    else
    {
        std::cout << "With the length of " << shortest->length_
                  << ", " << shortest->id_
                  << " is the shortest chapter in " << id
                  << "'s hierarchy." << std::endl;
    }


}

void Book::printParent(Params /*params*/) const
{


}

void Book::printSubchapters(Params /*params*/) const
{

}

Chapter *Book::findChapter(const std::string& id) const
{
    Chapter* chapter_ptr = nullptr;
    //check if chapter is in book
    Data::const_iterator iter = chapters_.find(id);
    if(iter != chapters_.end())
    {
        chapter_ptr = chapters_.at(id);
    }
    return chapter_ptr;


}

IdSet Book::vectorToIdSet(const std::vector<Chapter*>& container) const
{
    IdSet idset;
    for(const Chapter* p : container)
    {
        idset.insert(p->id_);
    }
    return idset;

}

//helper function to print the chapters and their subchapters
void Book::printChapterRecursive(const Chapter* ptr,
                                 const std::string &indentation,
                                 int index) const
{
    //print current chapter
    char sign = ptr->is_open_?'-' : '+';

    std::cout << sign << " " << indentation << index << ". "
                 << ptr->fullName_ << " ( " << ptr->length_
                 << " ) " << std::endl;
    //if chapter is open
    if(ptr->is_open_)
    {
        int counter = 1;
        for(Chapter* chapter : ptr->subchapters_)
        {
            printChapterRecursive(chapter, indentation + "  ", counter++);
        }
    }


}
//helper function to get and print the subchapters at different levels´
void Book::printsubchaptersRecursive(const std::vector<Chapter*>& ptr,
                                     IdSet& subs,
                                     int level) const
{

    int current =1;

    for(auto& chapter : ptr)
    {
        subs.insert(chapter->id_);
        //get the subchapter while the current level is less
        //than the user level input
        while(current < level)
        {
            printsubchaptersRecursive(chapter->subchapters_,subs,current++);

        }

    }
}
//helper function to get subchapters of a chapter
void Book::getsubchaptersRecursive(const std::vector<Chapter*>& ptr,
                                   std::vector<Chapter*>& subchapters) const
{

    for(auto& chapter : ptr)
    {
        //get the subchapter
        subchapters.push_back(chapter);
        getsubchaptersRecursive(chapter->subchapters_,subchapters);

    }

}





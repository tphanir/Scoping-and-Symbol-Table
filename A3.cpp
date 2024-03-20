#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define SIZE 29

class HashElement
{
    public:
        std::string key;
        std::string value;
        HashElement *next;
        HashElement(std::string key, std::string value)
        {
            this->key = key;
            this->value = value;
            next = NULL;
        }
};

class HashTable
{
    public:
        HashElement* Array[SIZE];
        HashTable *next;
        HashTable *prev;
        HashTable()
        {
            for(int i=0; i<SIZE; i++)
            {
                Array[i] = NULL;
            }
            next = NULL;
            prev = NULL;
        }
};


int HashFunction(std::string s)
{
    int ret_val;
    ret_val = 0;

    for(char c : s)
    {
        ret_val += static_cast<int>(c);
    }

    return ret_val % SIZE;
}

HashElement* SearchKey(std::string key, HashTable *ht)
{
    int index, found;
    HashElement *curr, *ans;

    index = HashFunction(key);
    curr = ht->Array[index];
    ans = NULL;
    found = 0;

    while(!found && curr != NULL)
    {
        if(curr->key == key)
        {
            found = 1;
            ans = curr;
        }
        else
        {
            curr = curr->next;
        }
    }
    return ans;
}

void InsertHashElement(HashTable *ht, HashElement *h)
{
    int index;
    HashElement *curr;

    curr = SearchKey(h->key, ht);
    if(curr != NULL)
    {
        std::cout << "Aldready declared" << std::endl;
    }
    else
    {
        index = HashFunction(h->key);
        curr = ht->Array[index];

        if(curr == NULL)
        {
            ht->Array[index] = h;
        }
        else
        {
            while(curr->next != NULL)
            {
                curr = curr->next;
            }
            curr->next = h;
        }
    }
}

HashTable* CreateHashTable()
{
    HashTable *x = new HashTable;
    x->prev = x->next = NULL;
    return x;
}



void PrintHashTable(HashTable *ht)
{
    HashElement *curr;
    for(int i=0; i<SIZE; i++)
    {
        std::cout << i << " : ";
        curr = ht->Array[i];
        while(curr != NULL)
        {
            std::cout << curr->key << " "<< curr->value;
            curr = curr->next;
        } 
        std::cout << std::endl;
    }
}

int main()
{
    std::fstream file("code.txt", std::ios::in);

    if(!file.is_open())
    {
        std::cout << "Error while opening the file." << std::endl; 
    }
    else
    {
        HashTable *hdr, *curr;
        std::string sentence;
        std::vector<std::string> words;
        std::string word;

        hdr = NULL;
        curr = hdr;

        while(file.good())
        {
            words.clear();
            getline(file, sentence);
            std::istringstream iss(sentence);
            while(iss >> word)
            {
                words.push_back(word);
            }
            
            if(words[0] == "begin")
            {
                if(hdr == NULL)
                {
                    hdr = CreateHashTable();
                    curr = hdr;
                }
                else
                {
                    HashTable *nptr = CreateHashTable();
                    nptr->next = NULL;
                    curr->next = nptr;
                    nptr->prev = curr;

                    curr = nptr;
                }
            }
            else if(words[0] == "assign")
            {
                HashElement *h = new HashElement(words[1], words[2]);
                InsertHashElement(curr, h);
            }
            
            else if(words[0] == "end")
            {
                HashTable *nptr = curr;
                curr = curr->prev;
                if(curr != NULL)
                {
                    curr->next = NULL;
                }
                delete nptr;
            }

            else if(words[0] == "print")
            {
                std::string key, value;
                int found;
                HashTable *ht;
                HashElement *ans;
            
                key = words[1];
                ht = curr;
                found = 0;

                while(ht != NULL && !found)
                {
                    ans = SearchKey(key, ht);
                    if( ans == NULL)
                    {
                        ht = ht->prev;
                    }
                    else
                    {
                        found = 1;
                    }  
                }
                if(found)
                {
                    value = ans->value;
                    std::cout << key << " : " << value << std::endl;
                }
                else
                {
                    std::cout << "Not found" << std::endl;
                }
            }
        }
        file.close();
    }   
}
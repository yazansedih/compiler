#include "./stlist.h"
/**
 * @brief STList::STList
 * default constructor, initialize head and Counter
 */
STList::STList()
{
    Counter = 0;
    Head = NULL;
}
/**
 * @brief STList::GetSymbol: search (linear search) the list and compare the name to the ones in the list
 * @param name : Given name to find
 * @return : If name is found found return NULL otherwise reaturn a pointer to the Node
 */
STEntry *STList::GetSymbol(char *name)
{
    STEntry *ste = Head;
    while (ste != NULL)
    {
        if (strcmp(name, ste->Name) == 0)
            return ste;
        ste = ste->Next;
    }
    return NULL;
}
/**
 * @brief STList::PutSymbol : Call GetSymbol, if name is alread in table return false, otherwise add it to the list
 *                           Add it as the first Entry, like a stack which is fastest. Update Counter and Head
 * @param name : Name of Entry (variable)
 * @param type : Type of variable
 * @return : True if the node is added and False if the Entry Already exists in the Table
 */
STEntry *STList::PutSymbol(char *name, STE_TYPE type)
{
    STEntry *ste = GetSymbol(name);
    if (ste)
    {
        return ste;
    }
    else
    {
        ste = new STEntry(name, type);
        ste->Next = Head;
        Head = ste;
        Counter++;
    }
    return ste;
}
/**
 * @brief STList::PrintAll : Prints All nodes in the list, use the print in the STEntry.
 * @param fp : File Pointer where to write
 */
void STList::PrintAll(FILE *fp)
{
    STEntry *ste = Head;
    while (ste != NULL)
    {
        ste->print(fp);
        ste = ste->Next;
    }
    fprintf(fp, "\n");
}
/**
 * @brief STList::Count returns Counter which is Number of Elements
 * @return
 */
int STList::Count()
{
    return Counter;
}
/**
 * @brief STList::Clear : deletes all Elements in the List and sets the counter to zero
 */
void STList::Clear()
{
    STEntry *ste;
    while (Head != NULL)
    {
        ste = Head;
        Head = Head->Next;
        delete ste;
    }
    Counter = 0;
}

STList::~STList()
{
    Clear();
}

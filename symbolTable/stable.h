#ifndef STABLE_H
#define STABLE_H
#include "./stlist.h"
#include <vector>

#define DEFAULT_SIZE 19
class STable
{
private:
    STList *Table; // Dynamic Array of size = Size
    void init(int size);
    unsigned long ElfHash(char *str);
    int foldCase = 0;
    int number_entries;       // Number of entries in table
    int number_probes = 0;    // Number of probes into table
    int number_hits = 0;      // Number of probes that immediately found entry
    int max_search_dist = -1; // Maximum entries searched
public:
    unsigned long Size;
    std::vector<STList *> slots; // Pointer to hash table array of entries

    STable *next;
    STable();
    ~STable();
    STable(unsigned long size, int foldCase);
    void Reset(unsigned long size);
    STable *enter_scope(STable *s); /*Inform the symbol table that the parser is entering a new scope */
    STable *exit_scope(STable *s);  /*Inform the symbol table that the parser is leaving a scope.*/
    // STEntry *GetSymbol(char *str);

    // STEntry *PutSymbol(char *str, STE_TYPE type);
    bool PutSymbol(char *name, STE_TYPE type);
    STEntry *FindInScope(char *name); // finds and prints the Entry if it exist
    STEntry *GetSymbol(char *name);
    void FindAndPrintEntry(char *name, FILE *fp);
    void PrintAll(FILE *fp);
    void Clear();
    void print_symbol_stats(FILE *fp);
};

#endif // STABLE_H
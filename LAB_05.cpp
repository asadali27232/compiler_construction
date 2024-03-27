// Loading CFGs
#include <iostream>
#include <cstring>

using namespace std;

struct Rule
{
        string *r;
        Rule *next;
};

struct Production
{
        string name;
        Production *next;
        Rule *first;
        Rule *last;
};

class CFG
{
public:
        Production *first;
        Production *last;
        CFG()
        {
                first = NULL;
                last = NULL;
        }
        void addProduction(string name)
        {
                Production *p = new Production();
                p->name = name;
                p->next = NULL;
                p->first = NULL;
                p->last = NULL;
                if (first == NULL)
                {
                        first = p;
                        last = p;
                }
                else
                {
                        last->next = p;
                        last = p;
                }
        }
        void addRule(string name, string rule)
        {
                Production *p = first;
                while (p != NULL)
                {
                        if (p->name == name)
                        {
                                Rule *r = new Rule();
                                r->r = new string(rule);
                                r->next = NULL;
                                if (p->first == NULL)
                                {
                                        p->first = r;
                                        p->last = r;
                                }
                                else
                                {
                                        p->last->next = r;
                                        p->last = r;
                                }
                                return;
                        }
                        p = p->next;
                }
        }
        void removeProduction(string name)
        {
                Production *p = first;
                Production *prev = NULL;
                while (p != NULL)
                {
                        if (p->name == name)
                        {
                                if (prev == NULL)
                                {
                                        first = p->next;
                                }
                                else
                                {
                                        prev->next = p->next;
                                }
                                delete p;
                                return;
                        }
                        prev = p;
                        p = p->next;
                }
        }
        void removeRule(string name, string rule)
        {
                Production *p = first;
                while (p != NULL)
                {
                        if (p->name == name)
                        {
                                Rule *r = p->first;
                                Rule *prev = NULL;
                                while (r != NULL)
                                {
                                        if (*r->r == rule)
                                        {
                                                if (prev == NULL)
                                                {
                                                        p->first = r->next;
                                                }
                                                else
                                                {
                                                        prev->next = r->next;
                                                }
                                                delete r;
                                                return;
                                        }
                                        prev = r;
                                        r = r->next;
                                }
                        }
                        p = p->next;
                }
        }
        // Delete a prodcution and all its rules
        void deleteProduction(string name)
        {
                Production *p = first;
                Production *prev = NULL;
                while (p != NULL)
                {
                        if (p->name == name)
                        {
                                Rule *r = p->first;
                                Rule *rprev = NULL;
                                while (r != NULL)
                                {
                                        Rule *temp = r;
                                        r = r->next;
                                        delete temp->r;
                                        delete temp;
                                }
                                if (prev == NULL)
                                {
                                        first = p->next;
                                }
                                else
                                {
                                        prev->next = p->next;
                                }
                                delete p;
                                return;
                        }
                        prev = p;
                        p = p->next;
                }
        }
        // Delete a rule from all productions
        void deleteRule(string rule)
        {
                Production *p = first;
                while (p != NULL)
                {
                        Rule *r = p->first;
                        Rule *prev = NULL;
                        while (r != NULL)
                        {
                                if (*r->r == rule)
                                {
                                        if (prev == NULL)
                                        {
                                                p->first = r->next;
                                        }
                                        else
                                        {
                                                prev->next = r->next;
                                        }
                                        delete r->r;
                                        delete r;
                                        r = prev->next;
                                }
                                else
                                {
                                        prev = r;
                                        r = r->next;
                                }
                        }
                        p = p->next;
                }
        }
        // Delete a CFG
        void deleteCFG()
        {
                Production *p = first;
                while (p != NULL)
                {
                        Rule *r = p->first;
                        Rule *rprev = NULL;
                        while (r != NULL)
                        {
                                Rule *temp = r;
                                r = r->next;
                                delete temp->r;
                                delete temp;
                        }
                        Production *temp = p;
                        p = p->next;
                        delete temp;
                }
                first = NULL;
                last = NULL;
        }
        // Remove null productions from CFG
        void removeNullProductions()
        {
                Production *p = first;
                while (p != NULL)
                {
                        Rule *r = p->first;
                        Rule *rprev = NULL;
                        while (r != NULL)
                        {
                                if (*r->r == "null")
                                {
                                        if (rprev == NULL)
                                        {
                                                p->first = r->next;
                                        }
                                        else
                                        {
                                                rprev->next = r->next;
                                        }
                                        delete r->r;
                                        delete r;
                                        r = rprev->next;
                                }
                                else
                                {
                                        rprev = r;
                                        r = r->next;
                                }
                        }
                        p = p->next;
                }
        }
        void print()
        {
                Production *p = first;
                while (p != NULL)
                {
                        cout << p->name << " -> ";
                        Rule *r = p->first;
                        while (r != NULL)
                        {
                                cout << *r->r << " | ";
                                r = r->next;
                        }
                        cout << endl;
                        p = p->next;
                }
        }
};

int main()
{
        CFG cfg;
        cfg.addProduction("S");
        cfg.addRule("S", "aSb");
        cfg.addRule("S", "ab");
        cfg.addRule("S", "bSa");
        cfg.addRule("S", "ba");
        cfg.print();

        cfg.removeProduction("S");
        cfg.print();

                return 0;
}
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
    Production *prev;
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
        // Check if production already exists
        Production *p = first;
        while (p != NULL)
        {
            if (p->name == name)
            {
                return;
            }
            p = p->next;
        }
        p = new Production();
        p->name = name;
        p->next = NULL;
        p->prev = NULL;
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
            p->prev = last;
            last = p;
        }
    }
    void addRule(string name, string rule)
    {
        // Check if rule exists in production

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
                break;
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
                if (p->next == NULL)
                {
                    last = prev;
                }
                else
                {
                    p->next->prev = prev;
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
                        if (r->next == NULL)
                        {
                            p->last = prev;
                        }
                        delete r->r;
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
    // Remove null rules from CFG which is represnted by null string
    void removeNullRules()
    {
        Production *p = first;
        while (p != NULL)
        {
            Rule *r = p->first;
            Rule *prev = NULL;
            while (r != NULL)
            {
                Rule *temp = r->next;
                if (*r->r == "null")
                {
                    if (prev == NULL)
                    {
                        p->first = r->next;
                    }
                    else
                    {
                        prev->next = r->next;
                    }
                    if (r == p->last)
                    {
                        p->last = prev;
                    }
                    delete r->r;
                    delete r;
                }
                else
                {
                    prev = r;
                }
                r = temp;
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
                cout << *r->r;
                if (r->next != NULL)
                {
                    cout << " | ";
                }
                r = r->next;
            }
            cout << endl;
            p = p->next;
        }

        cout << "---------------------------------" << endl;
    }

    
};

int main()
{
    CFG cfg;
    cfg.addProduction("A");
    cfg.addProduction("B");

    cfg.addRule("A", "aiB");
    cfg.addRule("A", "aiA");
    cfg.addRule("A", "ae");
    cfg.addRule("B", "giB");
    cfg.addRule("B", "g");
    cfg.addRule("B", "null");

    cfg.print();
    return 0;
}

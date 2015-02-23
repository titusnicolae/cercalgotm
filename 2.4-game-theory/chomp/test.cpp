#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <cassert>
using namespace std;

int min(int a,int b)
{
    if(a<b)
        return a;
    return b;
}

struct State
{
    public:
    int row[4]; // row[0] >= row[1] >= row[2] >= row[3]
    State()
    {
        for(int i=0;i<4;i++)
        {
            row[i]=0;
        }
    }
    State(int x0, int x1, int x2, int x3)
    {
        assert(x0 >= x1);
        assert(x1 >= x2);
        assert(x2 >= x3);

        row[0]=x0;
        row[1]=x1;
        row[2]=x2;
        row[3]=x3;
    }
    public:
    State subtract(int x, int y)
    {
        State ret = *this;
        for(int i=3; i>=y; i--)
        {
            ret.row[i] = min(ret.row[i], x);
        }
        return ret;
    }

    friend ostream& operator<<(ostream& os, const State& s)
    {
        /*
        for(int i=0;i<4;i++)
        {
            os<<s.row[i]<<" ";
        }
        os<<endl;
        */
        for(int i=3;i>=0;i--)
        {
            for(int j=0;j<s.row[i];j++)
            {
                os<<"*";
            }
            for(int j=s.row[i];j<7;j++)
            {
                os<<"_";
            }
            os<<endl;
        }
        return os;
    }
    bool operator<(const State& s) const
    {
        int c1=0, c2=0;
        for(int i=0;i<4;i++)
        {
            c1 = c1*10+row[i];
            c2 = c2*10+s.row[i];
        }
        return c1<c2;

        /*
        for(int i=0;i<4;i++)
        {
            if(row[i]<s.row[i])
            {
                return true;
            }
            else if(row[i]>s.row[i])
            {
                return false;
            }
        }
        return false;
        */
    }
    bool operator!=(const State& s) const
    {
        for(int i=0;i<4;i++)
        {
            if(row[i]!=s.row[i])
            {
                return true;
            }
        }
        return false;
    }
};

int main ()
{
    State s(4,3,2,1);

    queue<State> q;
    q.push(State(7,7,7,7));
    map<State, set<State> > transition;
    while(!q.empty())
    {
        State s = q.front(); q.pop();

        for(int y=0;y<4;y++)
        {
            for(int x=0;x<7;x++)
            {
                if(x!=0 or y!=0)
                {
                    State n = s.subtract(x, y);

                    if(n!=s)
                    {
                        transition[s].insert(n);
                        if(transition.find(n)==transition.end())
                        {
                            q.push(n);
                        }
                    }
                }
            }
        }
    }
    map<State, int> count;

    // state->state2
    for(auto& state: transition)
    {
        for(auto& state2: state.second)
        {
            count[state2]++;
        }
    }
    q.push(State(7,7,7,7));
    vector<State> sorted;
    while(!q.empty())
    {
        State top = q.front(); q.pop();
        sorted.push_back(top);
        for(auto& e: transition[top])
        {
            count[e]--;
            if(count[e]==0)
            {
                q.push(e);
            }
        }
    }
    map<State, bool> win;
    win[State(1,0,0,0)]=false;


    for(auto it=next(sorted.rbegin());it!=sorted.rend();it++)
    {
        bool noh = true;
        for(auto& e: transition[*it])
        {
            noh = noh and win[e];
        }
        win[*it]=not noh;
    }

    State ss(7,7,7,7);
    cout<<"From this move: "<<endl;
    cout<<ss<<endl;
    cout<<"Go to this move: "<<endl;
    for(auto& e: transition[ss])
    {
        if(win[e]==false)
        {
            cout<<e;
            break;
        }
    }

    return 0;
}

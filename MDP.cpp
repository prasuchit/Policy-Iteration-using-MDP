#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <time.h>
#include <algorithm>
using namespace std;

int gridsize = 4;
int nS = gridsize * gridsize, nA = 4;
int action[4] = {0, 1, 2, 3}; //{'N','E','W','S'};
double gamma = 0.99;
double R[16] = {0};
double T[16][16][4] ={{{0}}};
double V[16];
int policy[16];

double abs(double x)
{
    if (x < 0)
    {
        return -1 * x;
    }
    else
    {
        return x;
    }
}

double max1(double a, double b)
{
    if (a > b)
        return a;
    else
        return b;
}


int loc2s(int x, int y, int gridsize)
{
    x = max(0, min(gridsize - 1, x));
    y = max(0, min(gridsize - 1, y));
    int state = (y * gridsize + x);
    return state;
}

void evaluate_policy();
bool policy_improve();

int main(int argc, char *argv[])
{
    double temp;
    for (int i = 0; i < 16; i++)
    {
        if (i == 3 || i == 15)
            temp = 1;
        else if (i == 12)
            temp = -1;
        else if (i == 9)
            temp = -0.5;
        else
            temp = -0.05;
        R[i] = temp;
    }
    
    srand(time(0)); //initialize the random seed
    for (int i = 0; i < nS; i++)
    {
        int RandIndex = rand() % 4; //generates a random number between 0 and 3
        V[i] = 0;                   //V_prev[i]=0;
        if (i == 10)
            policy[i] = -1;
        else
            policy[i] = action[RandIndex];
    }
    int s = 0, nxts[4] = {};
    // cout<<"Hello World!";
    for (int y = 0; y < gridsize; y++)
    {
        for (int x = 0; x < gridsize; x++)
        {
            s = loc2s(x, y, gridsize);
            nxts[0] = loc2s(x, y + 1, gridsize); //N  // Creating a list of possible states you
            nxts[1] = loc2s(x + 1, y, gridsize); //E // could reach by performing the available
            nxts[2] = loc2s(x - 1, y, gridsize); //W // list of actions in each state.
            nxts[3] = loc2s(x, y - 1, gridsize); //S
            for (int a = 0; a < nA; a++)
            {
                for (int a2 = 0; a2 < nA; a2++)
                {
                    // Action North
                    if (a == a2)
                    {
                        temp = 0.85;
                    }
                    else if (a == 0 && a2 == 1)
                    {
                        temp = 0.075;
                    }
                    else if (a == 0 && a2 == 2)
                    {
                        temp = 0.075;
                    }
                    else if (a == 0 && a2 == 3)
                    {
                        temp = 0.0;
                    }
                    // Action East
                    else if (a == 1 && a2 == 0)
                    {
                        temp = 0.075;
                    }
                    else if (a == 1 && a2 == 2)
                    {
                        temp = 0.0;
                    }
                    else if (a == 1 && a2 == 3)
                    {
                        temp = 0.075;
                    }
                    // Action West
                    else if (a == 2 && a2 == 0)
                    {
                        temp = 0.075;
                    }
                    else if (a == 2 && a2 == 1)
                    {
                        temp = 0.0;
                    }
                    else if (a == 2 && a2 == 3)
                    {
                        temp = 0.075;
                    }
                    // Action South
                    else if (a == 3 && a2 == 0)
                    {
                        temp = 0.0;
                    }
                    else if (a == 3 && a2 == 1)
                    {
                        temp = 0.075;
                    }
                    else if (a == 3 && a2 == 2)
                    {
                        temp = 0.075;
                    }

                    T[nxts[a2]][s][a] = temp;
                }
            }
        }
    }
    
    int p = 0;

    bool policy_stable = false;

    while(!policy_stable){
    	evaluate_policy();
    	policy_stable = policy_improve();
        // policy_stable = true;
    }
    evaluate_policy();

    for(int i=0;i<nS;i++){

    		cout<<V[i]<<" "<<policy[i]<<endl;
    }
}

void evaluate_policy()
{
    double error = 0.2;
    double v;
    double R_val, T_val;
    while (error > 0.0001)
    {
        error = 0;
        for (int i = 0; i < nS; i++)
        {
            v = V[i];
            double sum = 0;
            for (int j = 0; j < nS; j++)
            {
                R_val = R[i];
                T_val = T[j][i][policy[i]];
                sum = sum + T_val * (R_val + gamma * V[j]);
            }
            V[i] = sum;
            error = max1(error, abs(v - V[i]));
        }
        // cout<<"I finished! Error is: ",error;
    }
}

bool policy_improve()
{
    bool policy_stable = true;
    int current_action;
    double R_val, T_val;

    for (int i = 0; i < nS; i++)
    {
        current_action = policy[i];
        double max = 0;
        double action = 0;

        for (int j = 0; j < nA; j++)
        {
            double sum = 0;
            for (int k = 0; k < nS; k++)
            {
                R_val = R[i];
                T_val = T[k][i][j];
                sum = sum + T_val * (R_val + gamma * V[k]);
            }

            if (sum > max)
            {
                max = sum;
                action = j;
            }
        }
        policy[i] = action;
        if (current_action != policy[i])
        {
            policy_stable = false;
        }
    }
    return policy_stable;
}
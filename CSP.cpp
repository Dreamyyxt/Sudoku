//
//  CSP.cpp
//  Sudoku
//
//  Created by cpsfaculty on 10/10/14.
//  Copyright (c) 2014 ___xiaotian yang___. All rights reserved.
//

#include <stdio.h>

#include "CSP.h"

/************************************** Below are the three functions you need to implement ***************************************/

/*Check whether current state satisfy the constraints*/
bool CSP::goalCheck(const State state)
{
    for (int i=0; i< 81;i++)
    {
        int row = i / 9;
        int col = i % 9;
        if (row==0||col==0) {
            int j;
            for(j = 0; j < 9; j++){
                if(state.values[row][j] == state.values[row][col] && j != col){
                    return false;
                }
                if(state.values[j][col] == state.values[row][col] && j != row){
                    return false;
                }
            }
        }
        
        if ((row==0||row==3||row==6)&&(col==0||col==3||col==6)) {
            int tempRow = row / 3 * 3;
            int tempCol = col / 3 * 3;
            for(int j = tempRow; j < tempRow + 3;j++){
                for(int k = tempCol; k < tempCol + 3; k++){
                    if(state.values[j][k] == state.values[row][col] && j != row && k != col){
                        return false;
                    }
                }
            }
        }
    }
        return true;
}



/*Update Domain for the forward checking*/
void CSP::updateDomain(const State state)
{
    //for all the cells
    for (int i=0; i< 81;i++)
    {
        int row = i / 9;
        int col = i % 9;
        if (state.values[row][col]!=0) {    //if the value is assigned
            int temp=state.values[row][col];
            for(int j = 0; j < 9; j++)
            {
                if(state.values[row][j] == 0&&j!=col)   //check the cells on the same row
                {
                    auto iter=std::find(variables[row][j].domain.begin(),
                                        variables[row][j].domain.end(),
                                        temp);
                    if (iter!=variables[row][j].domain.end())
                    {
                        variables[row][j].domain.erase(iter);
                    }
                }
                if (state.values[j][col]==0&&j!=row)
                {
                    auto iter=std::find(variables[j][col].domain.begin(),
                                        variables[j][col].domain.end(),
                                        temp);
                    if (iter!=variables[j][col].domain.end())
                    {
                            variables[j][col].domain.erase(iter);
                    }
                }
            }
                int tempRow = row / 3 * 3;
                int tempCol = col / 3 * 3;
                for(int j = tempRow; j < tempRow + 3;j++){
                    for(int k = tempCol; k < tempCol + 3; k++){
                        if(state.values[j][k]==0)
                        {
                            auto iter=std::find(variables[j][k].domain.begin(),
                                                variables[j][k].domain.end(),
                                                temp);
                            if (iter!=variables[j][k].domain.end()) {
                                variables[j][k].domain.erase(iter);
                        }
                    }
                }
            }
        }
        if (state.values[row][col]==0) {
            int temp;
            variables[row][col].domain.clear();
                for (int i=1; i<10; i++) {
                    
                    variables[row][col].domain.push_back(i);
                }
            if(random == 1)
                reshuffleDomain();
                for(int j = 0; j < 9; j++)
                {
                    if(state.values[row][j] != 0 && j!=col)   //check the cells on the same row
                    {
                        temp=state.values[row][j];
                        auto iter=std::find(variables[row][col].domain.begin(),
                                            variables[row][col].domain.end(),
                                            temp);
                        if (iter!=variables[row][col].domain.end())
                        {
                            variables[row][col].domain.erase(iter);
                        }
                    }
                    if (state.values[j][col]!=0&&j!=row)
                    {
                        temp=state.values[j][col];
                        auto iter=std::find(variables[row][col].domain.begin(),
                                            variables[row][col].domain.end(),
                                            temp);
                        if (iter!=variables[row][col].domain.end())
                        {
                            variables[row][col].domain.erase(iter);
                        }
                    }
                }
                int tempRow = row / 3 * 3;
                int tempCol = col / 3 * 3;
                for(int j = tempRow; j < tempRow + 3;j++){
                    for(int k = tempCol; k < tempCol + 3; k++){
                        if(state.values[j][k]!=0)
                        {
                            temp=state.values[j][k];
                            auto iter=std::find(variables[row][col].domain.begin(),
                                                variables[row][col].domain.end(),
                                                temp);
                            if (iter!=variables[row][col].domain.end()) {
                                variables[row][col].domain.erase(iter);
                            }
                        }
                    }
                }
        }
    }
}


/*Arc consistency use*/
void CSP::arcConsistency(const State state)
{
    updateDomain(state);
    queue<Arrow> q;
    for (int i=0; i<81; i++) {
        int row=i/9;
        int col=i%9;
        for (int j=0; j<9; j++) {
            if (j!=row) {
                Arrow temp=Arrow(i, 9*j+col);
                q.push(temp);
            }
            if (j!=col) {
                Arrow temp=Arrow(i,9*row+j);
                q.push(temp);
            }
        }
        int tempRow = row / 3 * 3;
        int tempCol = col / 3 * 3;
        for(int l = tempRow; l < tempRow + 3;l++){
            for(int k = tempCol; k < tempCol + 3; k++){
                if (l!=row&&k!=col) {
                    Arrow temp=Arrow(i, l*9+k);
                    q.push(temp);
                    }
                }
            }
        }
    //cout<<q.size()<<endl;
    while (!q.empty()) {
        Arrow arrow=q.front();
        q.pop();
        bool delete_sign=0;
        int row_tail=arrow.tail/9;
        int col_tail=arrow.tail%9;
        int row_head=arrow.head/9;
        int col_head=arrow.head%9;
        if (state.values[row_tail][col_tail]==0) {
            for (int x=0;x<variables[row_tail][col_tail].domain.size();x++)
            {
                int avail=0;
                for (int y=0;y<variables[row_head][col_head].domain.size();
                     y++) {
                    if (variables[row_tail][col_tail].domain[x]!=variables[row_head][col_head].domain[y]) {
                        avail=1;
                        break;
                    }
                }
                if ( avail==0) {
                    delete_sign=1;
                    variables[row_tail][col_tail].domain.erase(std::find(variables[row_tail][col_tail].domain.begin(), variables[row_tail][col_tail].domain.end(), variables[row_tail][col_tail].domain[x]));
                    x--;
                }

        }
        if (delete_sign==1) {
            for (int j=0; j<9; j++) {
                if (j!=row_tail) {
                    Arrow temp=Arrow( arrow.tail,9*j+col_tail);
                    q.push(temp);
                }
                if (j!=col_tail) {
                    Arrow temp=Arrow(arrow.tail,9*row_tail+j);
                    q.push(temp);
                }
                int tempRow = row_tail / 3 * 3;
                int tempCol = col_tail / 3 * 3;
                for(int l = tempRow; l < tempRow + 3;l++){
                    for(int k = tempCol; k < tempCol + 3; k++){
                        if (l!=row_tail&&k!=col_tail) {
                            Arrow temp=Arrow( arrow.tail,l*9+k);
                            q.push(temp);
                        }
                    }
                }
            }
        }
    }
}
}


/************************************************
 End of Assignment ***********************************************/




CSP::CSP()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignement = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }
            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    alg_opt = 1; //initially set it as back track
    
    srand(time(NULL));
    random = 0;
}


CSP::~CSP()
{
    
}




void CSP::setData(int *data)
{
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int idx = y * 9 + x;
            variables[y][x].assignement = data[idx]; //Initialize the assignment
            cur_state.values[y][x] = data[idx]; //Initizlize the current state
            
        }
    }
}

void CSP::clearData()
{
    /*Initially assign the domain, assignment for each variable and initialize the current state*/
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            variables[y][x].assignement = 0; //Initialize the assignment
            
            /*Initialize the domain*/
            variables[y][x].domain.clear();
            for(int i = 1; i <= 9; i++)
            {
                variables[y][x].domain.push_back(i);
            }
            
            cur_state.values[y][x] = 0; //Initizlize the current state
            
        }
    }
    
    /*Check whether a random domain is use*/
    if(random == 1)
        reshuffleDomain();
    
    repeating_list.clear();
    while(!assigned_variables.empty())
    {
        assigned_variables.pop();
        repeating_list.clear();
    }
    
}


void CSP::reshuffleDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::random_shuffle( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

void CSP::sortDomain()
{
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        std::sort( variables[y][x].domain.begin(), variables[y][x].domain.end() );
    }
}

/*Cancel last assignment*/
int CSP::goBack(int *chosen_cell)
{
    if(assigned_variables.size() > 0)
    {
        int cur_id = assigned_variables.top(); /*Remove last options*/
        assigned_variables.pop(); //pop out last option
        int y = cur_id / 9;
        int x = cur_id % 9;
        
        variables[y][x].assignement = 0; //assign the cell to zero
        cur_state.values[y][x] = 0; //update the assignment
        *chosen_cell = cur_id;
        
       // printf("(%d, %d)\n", y, x);
        if(alg_opt == 2)
        {
            updateDomain(cur_state);
        }
        else if (alg_opt == 3)
        {
            arcConsistency(cur_state);
        }
        
    }
    
    return goalCheck(cur_state);
    
}


bool CSP::arcCheckingOrder(int *chosen_cell)
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
        
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    /*First go through all the variables and do backtrack if there is no empty domain */
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}



/*arcChecking without ordering*/
bool CSP::arcChecking(int *chosen_cell)
{
    arcConsistency(cur_state);
    
    
    
    /*First go through all the variables and do backtrack if there is no empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                arcConsistency(cur_state);
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        arcConsistency(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            arcConsistency(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;

}



/*Forward Checking algorithm*/
bool CSP::forwardChecking(int *chosen_cell)
{
    updateDomain(cur_state); //the first step is based on current setting to update the domain
    
    
    
    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    /*If there is no variable that has empty domain, then assign variable here*/
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }

    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
    return false;
    
}


/*Forward Checking algorithm*/
bool CSP::forwardCheckingOrder(int *chosen_cell)
{

    updateDomain(cur_state); //the first step is based on current setting to update the domain
    
    
    
    /*First go through all the variables and do backtrack whether there is an empty domain */
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
        {
            int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                variables[y][x].assignement = 0;
                cur_state.values[y][x] = 0;
                updateDomain(cur_state);
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(temp_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        updateDomain(cur_state);
                        return false; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
        }
    }
    
    
    int count = 0;
    while (count < 81)
    {
        /*Find the index of minimum number of domain*/
        int min_idx = 0;
        int min_num = 10; //because the maximum number of domain is 10
        for(int i = 0; i < 81; i++)
        {
            int y = i / 9;
            int x = i % 9;
            if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
            {
                if (variables[y][x].domain.size() < min_num) {
                    min_idx = i;
                    min_num = variables[y][x].domain.size();
                }
            }
        }
        
        int y = min_idx / 9;
        int x = min_idx % 9;
        
        /*If there is any varable has not been assigned yet, assign it and return it*/
        if(cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
        {
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            
            updateDomain(cur_state); //Every time modify the assignment update the domain
            
            return false;
        }
        
        count++;
    }
    
    if(goalCheck(cur_state))
    {
        printf("find the goal\n");
        return true;
    }
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for(int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if(available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}

    return false;
    
}



/*Back Track to solve the proble*/
bool CSP::backTrack(int *chosen_cell)
{
    
  
    
    for(int i = 0; i < 81; i++)
    {
        int y = i / 9;
        int x = i % 9;
        
        
        /*If there is any varable has not been assigned yet, assign it and break*/
        if(cur_state.values[y][x] == 0)
        {
            
            /*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
            int id_min = 0;
            cur_state.values[y][x] = variables[y][x].domain[id_min];
            variables[y][x].assignement = variables[y][x].domain[id_min];
            assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
            repeating_list.push_back(cur_state); //make this state into the repeat_list
            *chosen_cell = 9 * y + x;
            return false;
            
        }
    }
    
    /*If all the the variable are assigned*/
    {
        if(assigned_variables.size() == 0)//reset all the variables if there are no any varaibles assigned yet
        {
            for(int i = 0; i < 81; i++)
            {
                assigned_variables.push(i);
            }
        }
        
        if(goalCheck(cur_state))
        {
            printf("find the goal\n");
            return true;
        }
        else
        {
            int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
            while (available_assignemnt == 0) {
                int cur_id = assigned_variables.top();
                int y = cur_id / 9;
                int x = cur_id % 9;
                
                
                for(int i = 0; i < variables[y][x].domain.size(); i++)
                {
                    State temp_state;
                    temp_state = cur_state;
                    temp_state.values[y][x] = variables[y][x].domain[i];
                    if (std::find(repeating_list.begin(), repeating_list.end(), temp_state)==repeating_list.end()) //if not in the repeating list
                    {
                        cur_state = temp_state;
                        variables[y][x].assignement = variables[y][x].domain[i];
                        repeating_list.push_back(cur_state);
                        available_assignemnt = 1;
                        *chosen_cell = cur_id;
                        break; //get out of the current varaible assignment
                    }
                }
                
                if(available_assignemnt == 0) //if all the domain values have been tried for current variable
                {
                    variables[y][x].assignement = 0;
                    cur_state.values[y][x] = 0;
                    assigned_variables.pop();
                    
                }
            }
            
            return false;
        }
    }
}
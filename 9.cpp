#include<bits/stdc++.h> 
using namespace std; 

// Function to find page faults using FIFO 
int pageFaults(int pages[], int n, int capacity) 
{ 
	//  We use 
	// an unordered_set so that we quickly check 
	// if a page is present in set or not 
	unordered_set<int> s; 

	// To store the pages in FIFO manner 
	queue<int> indexes; 

	int page_faults = 0; 
	for (int i=0; i<n; i++) 
	{ 
		// Check if the set can hold more pages 
		if (s.size() < capacity) 
		{ 
			// Insert it into set if not present  
			if (s.find(pages[i])==s.end()) 
			{ 
				// Insert the current page into the set 
				s.insert(pages[i]);  
				page_faults++; 

				// Push the current page into the queue 
				indexes.push(pages[i]); 
			} 
		} 

		else
		{ 
			// Check if current page is not already 
			// present in the set 
			if (s.find(pages[i]) == s.end()) 
			{ 
				// Store the first page in the 
				// queue to be used to find and 
				// erase the page from the set 
				int val = indexes.front(); 
				
				// Pop the first page from the queue 
				indexes.pop(); 

				// Remove the indexes page from the set 
				s.erase(val); 

				// insert the current page in the set 
				s.insert(pages[i]); 

				// push the current page into 
				// the queue 
				indexes.push(pages[i]); 

				page_faults++; 
			} 
		} 
	} 

	return page_faults; 
} 

// Driver code 
int main() 
{ 
	int pages[] = {7,5,1,2,4,5,0,9,1,1,2,3}; 
	int n = sizeof(pages)/sizeof(pages[0]); 
	int capacity = 9; 
	cout << pageFaults(pages, n, capacity); 
	return 0; 
} 

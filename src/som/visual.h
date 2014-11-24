#ifndef ann_som_visual_h
#define ann_som_visual_h

#include <iostream>
#include <iomanip>

#include "Grid.h"

std::ostream& operator << (std::ostream& os, const som::Grid& grid)
{
	for(unsigned rowIndex = 0; rowIndex < grid.rows(); ++rowIndex)
	{
		// Print leading padding
		for(unsigned p = 1; p < grid.rows() - rowIndex; ++p)
			os << "   ";
		// Print elements on diagonal from lower-left to top-right
		int i = rowIndex;
		unsigned j = 0;
		while((i >= 0) && (j < grid.columns()))
		{
			if (grid[i][j].empty()) 
				os << "   ...";
			else
				os << "   " << setw(3) << setfill('.') << grid[i][j].size();
			--i;
			++j;
		}
		os << '\n';
	}
	for(unsigned columnIndex = 1; columnIndex < grid.columns(); ++columnIndex)
	{
		// Print leading padding
		for(unsigned p = 1; p <= columnIndex; ++p)
			os << "   ";
		// Print elements on diagonal from lower-left to top-right
		int i = grid.rows() - 1;
		unsigned j = columnIndex;
		while(j < grid.columns() && i >= 0)
		{
			if (grid[i][j].empty()) 
				os << "   ...";
			else
				os << "   " << setw(3) << setfill('.') << grid[i][j].size();
			--i;
			++j;
		}
		os << '\n';
	}
	return os;
}



#endif // ann_som_visual_h

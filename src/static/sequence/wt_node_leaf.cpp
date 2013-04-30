/* wt_node_leaf.cpp
 * Copyright (C) 2008, Francisco Claude.
 * Copyright (C) 2011, Matthias Petri.
 *
 * wt_node_leaf
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <wt_node_leaf.h>

namespace cds_static
{

    wt_node_leaf::wt_node_leaf(uint symbol, size_t count) {
        this->symbol = symbol;
        this->count = count;
    }

    wt_node_leaf::wt_node_leaf() {}

    wt_node_leaf::~wt_node_leaf() {}

    size_t wt_node_leaf::rank(uint *symbol, size_t pos, uint l, wt_coder *c) const
    {
      /*if(symbol!=this->symbol) {
            return 0;
	    }*/
      //pos++;
        return pos+1;
    }

    size_t wt_node_leaf::select(uint *symbol, size_t pos, uint l, wt_coder *c) const
    {
      //if(symbol!=this->symbol) return (size_t)-1;
        if(pos==0 || pos>count) return (size_t)-1;
        return pos;
    }

    uint wt_node_leaf::access(size_t pos) const
    {
        return symbol;
    }

    uint wt_node_leaf::access(size_t pos, size_t &rank) const
    {
        rank = pos+1;
        return symbol;
    }

    pair<uint,size_t> wt_node_leaf::quantile_freq(size_t left,size_t right,uint q) const
    {
        return std::make_pair(symbol,right-left+1);
    }

    void wt_node_leaf::range_report(uint start, uint end, uint lowvoc, uint uppvoc, uint vocmin, uint vocmax, uint l, wt_coder *c, Mapper * am,vector<uint> &res) const
    {
    	 //printf("level leaf: %u\n",l);
    	 //printf("vocmin: %u\n", vocmin);
    	 //printf("vocmax: %u\n", vocmax);
    	 //printf("symbol: %u\n", symbol);

	    uint s = am->unmap(symbol);

    	      if (start >= end) {
    	    	//  printf("start >= end in leaf\n");
    	    	  return;
    	      }

    	      if (s >= lowvoc && s < uppvoc) {
    	    	  //cout << "answer: " << symbol << " " << end - start << endl;
    	    	  res.push_back(s);
    	    	  res.push_back(end - start);
    	      }
    	      //else {
    	    	//  printf("symbol %u not in alphabet [%u, %u)\n", symbol, lowvoc, uppvoc);
    	      //}
  
    }

    uint wt_node_leaf::next_value_pos_bin(uint number, uint start, uint end, uint vocmin, uint vocmax, uint l, wt_coder *c, uint *success) const {
	   // printf("symbol leaf: %u\n", symbol);
	    	    //printf("number leaf: %u\n", number);
	if ( start >= end ) {
		*success = 0;
		return 0;
	}

	if (symbol >= number) {
		*success = 1;
		return end-1;
	}

	return 0;
	/*
    	if (*number >= *ss) {
		printf("yuhuuu ready to go\n");
    		//printf("waaa of %u is %u %u diff: %u\n", number, start, end, end-start-1);
    		*success = 1;
    		return end-1; // to get the symbol, you only need return  "end-start-1"
    	}
    	else {
    		//printf("not in substring\n");
    		    			*success = 0;
    		    			return 0;
    	}*/

    }


    void wt_node_leaf::next_value_pos_huff(uint number, uint start, uint end, uint *min, uint *min_pos,uint *success) const {
	   // printf("leaf number: %u\n", number);
    	   // printf("leaf symbol: %u\n", symbol);

	    
    	if ( start >= end ) {
    		return;
    	}

    	if (symbol >= number) {
		if (*min > symbol) {
	        	 //   printf("leaf start: %u\n", start);
	        	   // printf("leaf end: %u\n", end);
			    //printf("leaf min: %u\n", symbol);
			*min = symbol;
			*min_pos = end;
		}

    	}

	
	/*
        	//printf("start: %u\nend: %u\nsymbol: %u\nnumber: %u\n", start, end, symbol, number);
        	uint *ss;

        	if ( start >= end ) {
    		printf("not in substring 2\n");
        			*success = 0;
        			return 0;
        		}
        	printf("symbol leaf: %u\n", symbol);
        	printf("unmap: %u\n", am->unmap(symbol));


        	ss = c->get_symbol(symbol);
    	
    if (c->getType() == WT_CODER_BINARY_HDR) {
    	*success = 1;
    	return end-1;
    }

    if (c->getType() == WT_CODER_HUFF_HDR) {
    	printf("symbol leaf huffman\n");
        	printf("ss: %u\n", *ss);
        	printf("number: %u\n", *number);
    	if (
    	*number >= *ss) {
    		*success = 1;
    		return end-1;
    	}
	
	
    }

    *success = 0;
    return 0; */
    }

    void wt_node_leaf::select_all(uint *symbol, uint l, wt_coder *c, vector<uint> &res) const {
    	uint i;
    	//printf("in leaf: %u\n", this->symbol);

    	//if (symbol == this->symbol) {
    	for (i = 0; i < this->count; i++) {
    		res.push_back(i);
    	}
    	//}
    }

    size_t wt_node_leaf::getSize() const
    {
        return sizeof(wt_node_leaf);
    }

    void wt_node_leaf::save(ofstream & fp) const
    {
        uint wr = WT_NODE_LEAF_HDR;
        saveValue(fp,wr);
        saveValue(fp,count);
        saveValue(fp,symbol);
    }

    wt_node_leaf * wt_node_leaf::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WT_NODE_LEAF_HDR) return NULL;
        wt_node_leaf * ret = new wt_node_leaf();
        ret->count = loadValue<size_t>(fp);
        ret->symbol = loadValue<uint>(fp);
        return ret;
    }
};

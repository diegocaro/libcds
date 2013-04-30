/* WaveletTree.cpp
 * Copyright (C) 2008, Francisco Claude, all rights reserved.
 * Copyright (C) 2008, Niko Välimäki.
 * Copyright (C) 2011, Matthias Petri.
 *
 * WaveletTree definition
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

#include <WaveletTree.h>

namespace cds_static
{

    WaveletTree::WaveletTree(const Array & a, wt_coder * coder, BitSequenceBuilder *bmb, Mapper *am) : Sequence(0) {
        bmb->use();
        this->n = a.getLength();
        this->length = n;
        this->am = am;
        am->use();
        this->c=coder;
        c->use();
        uint * symbols = new uint[n];
        for(size_t i=0;i<n;i++) {
            symbols[i] = am->map(a[i]);
        }
        root = new wt_node_internal(symbols, n, 0, c, bmb);
        delete [] symbols;
        bmb->unuse();
    }

    WaveletTree::WaveletTree(uint * symbols, size_t n, wt_coder * c, BitSequenceBuilder * bmb, Mapper * am, bool free) : Sequence(n) {
        bmb->use();
        this->n = n;
        for(size_t i=0;i<n;i++)
            symbols[i] = am->map(symbols[i]);
        this->am = am;
        am->use();
        this->c=c;
        c->use();
        root = new wt_node_internal(symbols, n, 0, c, bmb);
        if(free) {
            delete [] symbols;
        }
        else {
            for(size_t i=0;i<n;i++)
                symbols[i] = am->unmap(symbols[i]);
        }
        bmb->unuse();
    }

    WaveletTree::WaveletTree(uchar * symbols, size_t n, wt_coder * c, BitSequenceBuilder * bmb, Mapper * am, bool free) : Sequence(n) {
        bmb->use();
        this->n = n;
        for(size_t i=0;i<n;i++)
            symbols[i] = (uchar)am->map((uint)symbols[i]);
        this->am = am;
        am->use();
        this->c=c;
        c->use();
        uint *done = new uint[n/W+1];
        for (uint i = 0; i < n/W+1; i++)
            done[i] = 0;
        root = new wt_node_internal(symbols, n, 0, c, bmb, 0, done);
        delete [] done;
        if(free) {
            delete [] symbols;
        }
        else {
            for(uint i=0;i<n;i++)
                symbols[i] = (uchar)am->unmap((uint)symbols[i]);
        }
        bmb->unuse();
    }

    WaveletTree::WaveletTree():Sequence(0) {}

    WaveletTree::~WaveletTree() {
        delete root;
        am->unuse();
        c->unuse();
    }

    size_t WaveletTree::rank(uint symbol, size_t pos) const
    {
        uint * s = c->get_symbol(am->map(symbol));
        size_t ret = root->rank(s, pos, 0, c);
        delete [] s;
        return ret;
    }

    size_t WaveletTree::count(uint s) const
    {
      uint * s2 = c->get_symbol(am->map(s));
      size_t ret = root->rank(s2, length-1, 0, c);
      delete [] s2;
      return ret;
    }

    size_t WaveletTree::select(uint symbol, size_t pos) const
    {
      uint * s = c->get_symbol(am->map(symbol));
        uint ret = root->select(s, pos, 0, c);
        if(ret==((uint)-1)) { delete [] s; return (uint)-1;}
	delete [] s;
        return ret-1;
    }

    uint WaveletTree::access(size_t pos) const
    {
        return am->unmap(root->access(pos));
    }

    uint WaveletTree::quantile(size_t left,size_t right,uint q) const
    {
        return quantile_freq(left,right,q).first;
    }

    pair<uint,size_t> WaveletTree::quantile_freq(size_t left,size_t right,uint q) const
    {
        /* q=1 -> q=0 */
        q--;

        pair<uint,size_t> res = root->quantile_freq(left,right,q);
        return std::make_pair( am->unmap(res.first) , res.second );
    }
    
    void WaveletTree::range_report(uint start, uint end, vector<uint> &res) const {
    	uint max_sigma;
	    
    	if ( am->getType() == MAPPER_NONE_HDR) {
    		max_sigma = 1u << c->depth();
		
    		//who cares about max sigma in huffman???? we need to traverse the all tree :(
    	/*	else if (c->getType() == WT_CODER_HUFF_HDR) {
    			max_sigma = c->depth() * 2;
    		}
    		*/
    	}
	    
    	if ( am->getType() == MAPPER_CONT_HDR) {
    		max_sigma = am->length();
    	}
	
    	//printf("max_sigma: %u\n", max_sigma);
	  
	  root->range_report(start, end, 0, max_sigma, 0, max_sigma, 0, c, am, res);
          /*for(uint i = 0; i < res.size(); i+=2) {
        	res[i] = am->unmap(res[i]);
          }*/

    }

    void WaveletTree::range_report(uint start, uint end, uint lowvoc, uint uppvoc, vector<uint> &res) const
    {
	    // We can't use HUFFMAN with CODER_BINARY, because we arent able to
	    // to map symbols that arent in the sequence (but can belong to sigma)
	    
	    
	uint max_sigma;
	    
	if ( am->getType() == MAPPER_NONE_HDR) {
		max_sigma = 1u << c->depth();
		
		//who cares about max sigma in huffman???? we need to traverse the all tree :(
	/*	else if (c->getType() == WT_CODER_HUFF_HDR) {
			max_sigma = c->depth() * 2;
		}
		*/
	}
	    
	if ( am->getType() == MAPPER_CONT_HDR) {
		max_sigma = am->length();
	}
	
	//printf("max_sigma: %u\n", max_sigma);
	
	root->range_report(start, end, lowvoc, uppvoc, 0, max_sigma, 0, c, am, res);
    }

    uint WaveletTree::next_value_pos(uint number, uint start, uint end) const {
	    // We can't use HUFFMAN with CODER_BINARY, because we arent able to
	    // to map symbols that arent in the sequence (but can belong to sigma)
    	uint success = 0;
    	uint pos;
	
	uint mnumber;
	mnumber = am->succesor(number);
	
//	printf("begin number: %u\n", number);
//	printf("begin map number: %u\n", mnumber);
	
	if (mnumber+1 == 0) {
		//there isnt a successor
		return this->n;
	}

	if (c->getType() == WT_CODER_BINARY_HDR) {
		uint max_sigma;
	    
		if ( am->getType() == MAPPER_NONE_HDR)
			max_sigma = 1u << c->depth();
	    
		if ( am->getType() == MAPPER_CONT_HDR) 
			max_sigma = am->length();
		/*
		if (mnumber > max_sigma) {
			return this->n;
		}
		*/
		
		pos = root->next_value_pos_bin(mnumber, start, end, 0, max_sigma, 0, c, &success);
	}
	else if (c->getType() == WT_CODER_HUFF_HDR) {
		uint min;
		uint min_pos;
		
		min = -1;
		
		root->next_value_pos_huff(mnumber, start, end, &min, &min_pos, &success);
		//printf("min: %u -> %u\n", min, am->unmap(min));
		//printf("min pos: %u\n", min_pos);
		uint * s = c->get_symbol(min);
		success = 1;
		pos = root->select(s, min_pos, 0, c)-1;
		delete [] s;
	}
    	//printf("success: %u\n", success);

    	if (success)
    		return pos;
    	else
    		return this->n;

    }

    void WaveletTree::select_all(uint symbol, vector<uint> &res) const {
	uint * s = c->get_symbol(am->map(symbol));
  //  	root->select_all(s, 0, c, res);

    	root->select_all(s, 0, c, res);

	delete [] s;

    }


    uint WaveletTree::access(size_t pos, size_t &rank) const
    {
        return am->unmap(root->access(pos, rank));
    }

    size_t WaveletTree::getSize() const
    {
      // cout << "sizeof(WT): " << sizeof(WaveletTree) << endl;
      // cout << "root: " << root->getSize() << endl;
      // cout << "am: " << am->getSize() << endl;
      // cout << "cdr: " << c->getSize() << endl;
      return sizeof(WaveletTree)+root->getSize()+am->getSize()+c->getSize();
    }

    void WaveletTree::save(ofstream & fp) const
    {
        uint wr = WVTREE_HDR;
        saveValue(fp, wr);
        saveValue(fp, n);
        c->save(fp);
        am->save(fp);
        root->save(fp);
    }

    WaveletTree * WaveletTree::load(ifstream & fp) {
        uint rd = loadValue<uint>(fp);
        if(rd!=WVTREE_HDR) return NULL;
        WaveletTree * ret = new WaveletTree();
        ret->n = loadValue<size_t>(fp);
	ret->length = ret->n;
        ret->c = wt_coder::load(fp);
        ret->c->use();
        assert(ret->c!=NULL);
        ret->am = Mapper::load(fp);
        ret->am->use();
        assert(ret->am!=NULL);
        ret->root = wt_node::load(fp);
        assert(ret->root!=NULL);
        return ret;
    }

};

/**
 *    SequenceWaveletTreeExample.cpp
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <libcdsBasics.h>
#include <BitSequence.h>
#include <Mapper.h>
#include <Sequence.h>

using namespace std;
using namespace cds_static;



int main(int argc, char **argv) {

	size_t N;
	uint s;
	cin >> N;
	uint * seq = new uint[N];
	for(size_t i=0;i<N;i++) {
		uint v;
		cin >> seq[i];
	}
	BitSequenceBuilderRG bbb(20);
  MapperCont ru(seq,N,bbb);
  ru.use();

	WaveletTree * wt1 = new WaveletTree(seq, N,
			new wt_coder_binary(seq, N,	&ru),
			new BitSequenceBuilderRG(20),
			&ru);


	WaveletTree * wt2 = new WaveletTree(seq, N,
			new wt_coder_huff(seq, N, &ru),
			new BitSequenceBuilderRG(20),
			&ru);
	cout << "size wt1 = " << wt1->getSize() << " bytes" << endl;
	cout << "size wt2 = " << wt2->getSize() << " bytes" << endl;


	for( size_t i = 0; i < N; i++) {

		assert(seq[i] == wt1->access(i));
		assert(seq[i] == wt2->access(i));

		assert(wt1->rank(seq[i], i) == wt2->rank(seq[i], i));

	  cout << i << "\t";
	  cout << seq[i] ;
	  /*cout << "\t"  << wt1->access(i);
	  cout << "\t"  << wt1->rank(seq[i],i);
	  //cout << "\t"  << wt1->next_value_pos(88888880, 0, 4);

	  //cout << "\t"  << wt1->rank(seq[i]+i,i);

		 */
	  cout << endl;
	}

	uint i;
	vector<uint> res1;
	vector<uint> res2;

	
printf("select init\n");
  wt1->select_all(2, res1);
    wt2->select_all(2, res2);
  printf("select end\n");
  for(i = 0; i < res1.size(); i ++) {
	  printf("res1: %u\n", res1[i]);
	   printf("res2: %u\n", res2[i]);
  }
  res1.clear();
    res2.clear();
  return 0;
  





	uint from, to;
	uint a,b,b1,b2 ;

	while (1) {
		cin >> a;
		cin >> b;
		cin >> from;
		cin >> to;
		if (from == to && to == 0) break;
		printf("Getting range_report of substring [%u...%u) in alphabet [%u, %u)\n", a, b, from, to);
		wt1->range_report(a, b, from, to, res1);
		wt2->range_report(a, b, from, to, res2);

		assert(res1.size() == res2.size());

		printf("answer size: %u\n", res1.size());
		//sort(res1.begin(), res1.end());
		//sort(res2.begin(), res2.end());
		for(i = 0; i < res1.size(); i += 2) {
			printf("res1: %u -> %u\n", res1[i], res1[i+1]);
			printf("res2: %u -> %u\n", res2[i], res2[i+1]);
			//assert(res1[i] == res2[i]);
			//assert(res1[i+1] == res2[i+1]);
		}
		res1.clear();
		res2.clear();

	}

	while(1) {
		cin >> a;
		cin >> from;
		cin >> to;

		if (a == from && a == 0) break;

		printf("next_value_pos of %u in substring [%u...%u)\n", a, from, to);
		b1 = wt1->next_value_pos(a,from, to);
		printf("b1: %u\n", b1);
		b2 = wt2->next_value_pos(a,from, to);
		printf("b2: %u\n", b2);
		assert(b1 == b2);


		if (b1 < wt1->getLength()) {
			printf("pos1: %u => value: %u\n", b1, seq[b1]);
		}
		else {
			printf("successor of %u not found\n", a);
		}


		if (b2 < wt2->getLength()) {
			printf("pos2: %u => value: %u\n", b2, seq[b2]);
		}
		else {
			printf("successor of %u not found\n", a);
		}


	}
	delete wt1;
	delete []seq;
	return 0;
}


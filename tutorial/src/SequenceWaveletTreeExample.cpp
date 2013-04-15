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
  cout << "Length: ";
  cin >> N;
  uint * seq = new uint[N];
  for(size_t i=0;i<N;i++) {
    uint v;
    cout << "Element at position " << i << ": ";
    cin >> seq[i];
  }
  WaveletTree * wt1 = new WaveletTree(seq, N, 
				      new wt_coder_binary(seq, N,	new MapperNone()),
				      new BitSequenceBuilderRG(20), 
				      new MapperNone());
  cout << "size = " << wt1->getSize() << " bytes" << endl;


  for( size_t i = 0; i < N; i++) {
	  cout << i << "\t";
    cout << seq[i] ;
    cout << "\t"  << wt1->access(i);
    cout << "\t"  << wt1->rank(seq[i],i);
    //cout << "\t"  << wt1->next_value_pos(88888880, 0, 4);
    //cout << "\t"  << wt1->rank(seq[i]+i,i);
    cout << endl;
    
    
  }






uint i;
  vector<uint> res;
  printf("select init\n");
  wt1->select_all(77777771, res);
    printf("select end\n");
  for(i = 0; i < res.size(); i ++) {
	  printf("res: %u\n", res[i]);
  }
  res.clear();
  return 0;


  
  uint from, to;
  uint a,b ;

  while (1) {
	  cin >> a;
	  cin >> b;
  cin >> from;
  cin >> to;
if (from == to && to == 0) break;
printf("Getting range_report of substring [%u...%u) in alphabet [%u, %u)\n", a, b, from, to);
  wt1->range_report(a, b, from, to, &res);
  printf("answer size: %u\n", res.size());

  for(i = 0; i < res.size(); i += 2) {
	  printf("res: %u -> %u\n", res[i], res[i+1]);
  }
  res.clear();

  }

  while(1) {
  cin >> a;
  cin >> from;
  cin >> to;

  if (a == from && a == 0) break;


  printf("next_value_pos of %u in substring [%u...%u)\n", a, from, to);
  b = wt1->next_value_pos(a,from, to);
  if (b < wt1->getLength()) {
  printf("pos: %u => value: %u\n", b, seq[b]);
  }
  else {
	  printf("successor of %u not found\n", a);
  }


  }
  delete wt1;
  delete []seq;
  return 0;
}


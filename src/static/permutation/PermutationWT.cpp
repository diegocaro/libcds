/**
 *    PermutationWT.cpp
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

PermutationWT::PermutationWT(uint *perm, size_t len) {
  uint * seq = new uint[len];
  uint * marker = new uint[uint_len(len,1)];
  for(size_t i=0;i<uint_len(len,1);i++)
    marker[i] = 0;

  runs = 0;
  uint last = perm[0];
  seq[perm[0]] = 0;

  for(size_t i=1;i<len;i++) {
    if(last > perm[i]) {
      runs++;
      last = perm[i];
    }
    seq[perm[i]] = runs;
  }

  wt = new WaveletTree(seq, len, new wt_coder_huff(seq, len, new MapperNone()), new BitSequenceBuilderRG(20), new MapperNone());
  marks = new BitSequenceBuilderRG(marker, len, 20);
  delete [] seq;
}

PermutationWT::~PermutationWT(){ 
  delete wt;
}

uint PermutationWT::pi(uint k) {
  uint v = (uint)marks->rank1(k);
  return (uint)wt->select(v, v-marks->select1(v)+1);
}

uint PermutationWT::revpi(uint k) {
  size_t val = 0;
  uint s = wt->access(k, val);
  return marks->select1(s+1) + val - 1;
}

size_t PermutationWT::getSize() {
  return marks->getSize()+wt->getSize()+sizeof(PermutationWT);
}

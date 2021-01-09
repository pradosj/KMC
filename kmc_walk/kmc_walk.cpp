#include <unistd.h>
#include "kseq.h"
KSEQ_INIT(int, read)

#include "../kmc_api/kmc_file.h"

int main(int argc, char* argv[]) {
  if (argc!=2) {printf("usage: cat in.fasta | %s db.kmc3 > out.fasta\n",argv[0]);return 1;}
  
  // load the DB
	CKMCFile kmers;
  if (!kmers.OpenForRA(argv[1])) return 2;

  std::vector<uint32> counters;
  kseq_t* seq = kseq_init(STDIN_FILENO);
  while(kseq_read(seq)>=0) {
    // query kmer counts
    kmers.GetCountersForRead(seq->seq.s,counters);
    
    // output
    printf(">%s %s count:%s:",seq->name.s,seq->comment.s,argv[1]);
    if (!counters.empty()) printf("%d",counters[0]);
    for(auto i=1;i<counters.size();i++) printf(",%d",counters[i]);
    printf("\n%s\n",seq->seq.s);
  }
  
  // clean
  kseq_destroy(seq);
	kmers.Close();
	return 0;
}




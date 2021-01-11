#include <unistd.h>
#include "kseq.h"
KSEQ_INIT(int, read)

#include "../kmc_api/kmc_file.h"

int main(int argc, char* argv[]) {
  if (argc!=2) {printf("usage: cat in.fasta | %s db.kmc3 > out.wig\n",argv[0]);return 1;}
  
  // load the DB
	CKMCFile kmers;
  if (!kmers.OpenForRA(argv[1])) return 2;

  std::vector<uint32> counters;
  kseq_t* seq = kseq_init(STDIN_FILENO);
  while(kseq_read(seq)>=0) {
    // query kmer counts
    kmers.GetCountersForRead(seq->seq.s,counters);
    
    // output
    printf("fixedStep chrom=%s start=1 step=1\n",seq->name.s);
    for(auto i:counters) printf("%d\n",i);
  }
  
  // clean
  kseq_destroy(seq);
	kmers.Close();
	return 0;
}




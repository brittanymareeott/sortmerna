#pragma once
/**
 * FILE: readstats.hpp
 * Created: Nov 06, 2017 Mon
 *
 * Collective Statistics for all Reads. Encapsulates old 'compute_read_stats' logic and results
 */

#include <stdint.h>
#include <string>

#include "common.hpp"
#include "options.hpp"

struct Readstats {
	Runopts & opts;

	// Synchronized - Compute in worker thread (per read) - shared
	uint32_t min_read_len; // length of the shortest Read in the Reads file. 
	uint32_t max_read_len; // length of the longest Read in the Reads file.
	uint64_t total_reads_mapped; // total number of reads mapped passing E-value threshold. Computed in 'compute_lis_alignment' in a worker thread i.e. per read.
	char filesig = '>';
	std::string suffix;

	// TODO: move to Readrec and get rid of this vector
	//std::vector<bool> read_hits; // flags if a read was aligned i.e. match found. Each value represents a read. True when the read was matched/aligned.

	// TODO: move to Readrec and get rid of this vector
	// bits representing all reads. An accepted read with < %id and < %coverage is set to false (0)
	//std::vector<bool> read_hits_denovo;

	// TODO: move to Readrec and get rid of this vector
	// array of uint16_t to represent all reads, if the read was aligned with a maximum SW score, its number of alignments is incremeted by 1
	//uint16_t *read_max_SW_score; // SW (Smith-Waterman) Max SW score of a read -> Readrec.max_SW_score

	// Non-synchronized - Compute once by calculate
	uint64_t number_total_read; // total number of reads in file.
	off_t    full_file_size; // the size of the full reads file (in bytes).
	uint64_t full_read_main; // total number of nucleotides in all reads.

	Readstats(Runopts & opts)
		:
		opts(opts),
		min_read_len(READLEN),
		max_read_len(0),
		total_reads_mapped(0)
	{
		calcSuffix();
		opts.exit_early = check_file_format();
	}

	~Readstats() {}

	void calculate(); // calculate statistics from readsfile see "compute_read_stats"
	bool check_file_format();
	void calcSuffix();
	// called from Main thread once when 'number_total_read' is known
	//void set_read_hits() {
	//	read_hits.resize(2*number_total_read); // why twice the number of reads? Because original **reads array has 2 lines per read: header and sequence.
	//	std::fill(read_hits.begin(), read_hits.end(), false);
	//}
	//void set_read_hits_denovo() {
	//	read_hits_denovo.resize(2*number_total_read);
	//	std::fill(read_hits_denovo.begin(), read_hits_denovo.end(), true);
	//}
	//void set_read_max_SW_score() {
	//	read_max_SW_score = new uint16_t[2*number_total_read];
	//	memset(read_max_SW_score, 0, sizeof(uint16_t)*(2*number_total_read));
	//}
}; // ~struct Readstats

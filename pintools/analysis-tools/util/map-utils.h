/*
 * map-utils.h
 *
 *  Created on: Sep 14, 2015
 *      Author: conor
 */

#ifndef ANALYSIS_TOOLS_UTIL_MAP_UTILS_H_
#define ANALYSIS_TOOLS_UTIL_MAP_UTILS_H_

namespace mapUtils {
	template <typename A, typename B>
	std::multimap<B, A> flip_map(std::map<A,B> & src) {
		std::multimap<B,A> dst;

		for(auto &kv : src) {
			dst.insert(std::pair<B, A>(kv.second, kv.first));
		}
		return dst;
	}
}

#endif /* ANALYSIS_TOOLS_UTIL_MAP_UTILS_H_ */

/*
 * cache-line-access.h
 *
 *  Created on: Jun 22, 2015
 *      Author: conor
 */

#ifndef CACHE_LINE_ACCESS_H_
#define CACHE_LINE_ACCESS_H_


struct CacheLineAccess {
	std::string varInfo;
	std::string accessSite;
	size_t timestamp;
	unsigned short accessSize;
};


#endif /* CACHE_LINE_ACCESS_H_ */

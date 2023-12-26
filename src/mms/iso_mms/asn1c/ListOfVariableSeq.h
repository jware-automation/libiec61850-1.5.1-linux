/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_ListOfVariableSeq_H_
#define	_ListOfVariableSeq_H_


#include <asn_application.h>

/* Including external dependencies */
#include "VariableSpecification.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AlternateAccess;

/* ListOfVariableSeq */
typedef struct ListOfVariableSeq {
	VariableSpecification_t	 variableSpecification;
	struct AlternateAccess	*alternateAccess	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ListOfVariableSeq_t;

/* Implementation */
LIB61850_INTERNAL extern asn_TYPE_descriptor_t asn_DEF_ListOfVariableSeq;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AlternateAccess.h"

#endif	/* _ListOfVariableSeq_H_ */

#include "ternary_fields.h"

#include "../platform/memory.h"

/*
* Initializers
*/
inline utern_t* __init_utern8(uint8_t value, uint8_t mask){
	utern8_t* tern = (utern8_t*)platform_malloc_shared(sizeof(utern8_t));

	if(!tern)
		return NULL;

	tern->type = UTERN8_T;
	tern->value = value;
	tern->mask = mask;
	return (utern_t*)tern;
}
inline utern_t* __init_utern16(uint16_t value, uint16_t mask){
	utern16_t* tern = (utern16_t*)platform_malloc_shared(sizeof(utern16_t));

	if(!tern)
		return NULL;

	tern->type = UTERN16_T;
	tern->value = value;
	tern->mask = mask;
	return (utern_t*)tern;
}
inline utern_t* __init_utern32(uint32_t value, uint32_t mask){
	utern32_t* tern = (utern32_t*)platform_malloc_shared(sizeof(utern32_t));

	if(!tern)
		return NULL;
	
	tern->type = UTERN32_T;
	tern->value = value;
	tern->mask = mask;
	return (utern_t*)tern;
}
inline utern_t* __init_utern64(uint64_t value, uint64_t mask){
	utern64_t* tern = (utern64_t*)platform_malloc_shared(sizeof(utern64_t));

	if(!tern)
		return NULL;
	
	tern->type = UTERN64_T;
	tern->value = value;
	tern->mask = mask;
	return (utern_t*)tern;
}
inline utern_t* __init_utern128(uint64_t value[2], uint64_t mask[2]){ //uint128_t funny!
	utern128_t* tern = (utern128_t*)platform_malloc_shared(sizeof(utern128_t));
	
	if(!tern)
		return NULL;
	
	tern->type = UTERN128_T;
	tern->value[UTERN128_LOW] = value[UTERN128_LOW];
	tern->value[UTERN128_HIGH] = value[UTERN128_HIGH];
	tern->mask[UTERN128_LOW] = mask[UTERN128_LOW];
	tern->mask[UTERN128_HIGH] = mask[UTERN128_HIGH];
	return (utern_t*)tern;
}

/*
* Single destructor
*/
rofl_result_t __destroy_utern(utern_t* utern){
	platform_free_shared(utern);
	//FIXME: maybe check returning value
	return ROFL_SUCCESS; 
}	


/*
* Comparison 
*/

inline bool __utern_compare8(const utern8_t* tern, const uint8_t value){
	return (tern->value & tern->mask) == (value & tern->mask); 
}
inline bool __utern_compare16(const utern16_t* tern, const uint16_t value){
	return (tern->value & tern->mask) == (value & tern->mask); 
}
inline bool __utern_compare32(const utern32_t* tern, const uint32_t value){
	return (tern->value & tern->mask) == (value & tern->mask); 
}

inline bool __utern_compare64(const utern64_t* tern, const uint64_t value){
	return (tern->value & tern->mask) == (value & tern->mask); 
}

inline bool __utern_compare128(const utern128_t* tern, const uint64_t value[2]){
	return ( (tern->value[UTERN128_LOW] & tern->mask[UTERN128_LOW]) == (value[UTERN128_LOW] & tern->mask[UTERN128_LOW]) ) &&
		( (tern->value[UTERN128_HIGH] & tern->mask[UTERN128_HIGH]) == (value[UTERN128_HIGH] & tern->mask[UTERN128_HIGH]) ); 
}

/*
* Contained 
*/
//Extensive tern is a more generic (with a less restrictive mask or equal) to tern
inline bool __utern_is_contained8(const utern8_t* extensive_tern, const utern8_t* tern){
	if(((extensive_tern->mask ^ tern->mask) & extensive_tern->mask) > 0)
		return false;
	return (extensive_tern->value & extensive_tern->mask) == (tern->value & extensive_tern->mask); 
}
inline bool __utern_is_contained16(const utern16_t* extensive_tern, const utern16_t* tern){
	if(((extensive_tern->mask ^ tern->mask) & extensive_tern->mask) > 0)
		return false;
	return (extensive_tern->value & extensive_tern->mask) == (tern->value & extensive_tern->mask);
}
inline bool __utern_is_contained32(const utern32_t* extensive_tern, const utern32_t* tern){
	if(((extensive_tern->mask ^ tern->mask) & extensive_tern->mask) > 0)
		return false;
	return (extensive_tern->value & extensive_tern->mask) == (tern->value & extensive_tern->mask);
}
inline bool __utern_is_contained64(const utern64_t* extensive_tern, const utern64_t* tern){
	if(((extensive_tern->mask ^ tern->mask) & extensive_tern->mask) > 0)
		return false;
	return (extensive_tern->value & extensive_tern->mask) == (tern->value & extensive_tern->mask);
}
inline bool __utern_is_contained128(const utern128_t* extensive_tern, const utern128_t* tern){
	if((((extensive_tern->mask[UTERN128_LOW] ^ tern->mask[UTERN128_LOW]) & extensive_tern->mask[UTERN128_LOW]) > 0 ) || 
		(((extensive_tern->mask[UTERN128_HIGH] ^ tern->mask[UTERN128_HIGH]) & extensive_tern->mask[UTERN128_HIGH]) > 0 ) )
		return false;
	return ( (extensive_tern->value[UTERN128_LOW] & extensive_tern->mask[UTERN128_LOW]) == (tern->value[UTERN128_LOW] & extensive_tern->mask[UTERN128_LOW]) &&
		(extensive_tern->value[UTERN128_HIGH] & extensive_tern->mask[UTERN128_HIGH]) == (tern->value[UTERN128_HIGH] & extensive_tern->mask[UTERN128_HIGH])	);
}
/*
* Check if two ternary values are equal
*/
inline bool __utern_equals8(const utern8_t* tern1, const utern8_t* tern2){
	return (tern1->value == tern2->value) && (tern1->mask == tern2->mask);
}
inline bool __utern_equals16(const utern16_t* tern1, const utern16_t* tern2){
	return (tern1->value == tern2->value) && (tern1->mask == tern2->mask);
}
inline bool __utern_equals32(const utern32_t* tern1, const utern32_t* tern2){
	return (tern1->value == tern2->value) && (tern1->mask == tern2->mask);
}
inline bool __utern_equals64(const utern64_t* tern1, const utern64_t* tern2){
	return (tern1->value == tern2->value) && (tern1->mask == tern2->mask);
}
inline bool __utern_equals128(const utern128_t* tern1, const utern128_t* tern2){
	return (tern1->value[UTERN128_LOW] == tern2->value[UTERN128_LOW]) && (tern1->mask[UTERN128_LOW] == tern2->mask[UTERN128_LOW]) &&
			(tern1->value[UTERN128_HIGH] == tern2->value[UTERN128_HIGH]) && (tern1->mask[UTERN128_HIGH] == tern2->mask[UTERN128_HIGH]);
}

//Ternary alike functions. Tern2 MUST always have more restrictive mask
inline utern_t* __utern8_get_alike(const utern8_t tern1, const utern8_t tern2){
	//TODO: there might be more efficient impl. maybe erasing 1s in diff... but dunno
	uint8_t diff, new_mask;

	diff = ~( 
			(tern1.value & tern1.mask)	
			^
			(tern2.value & tern2.mask)
			);
	//erase right 1.
	for(new_mask=0xFF;new_mask;new_mask=new_mask<<1)
		if((diff&new_mask) == new_mask) break; 

	if(tern1.mask<new_mask || tern2.mask < new_mask )
		return NULL;
	
	if(new_mask)
		return __init_utern8(tern1.value,new_mask);

	return NULL;
}
inline utern_t* __utern16_get_alike(const utern16_t tern1, const utern16_t tern2){
	//TODO: there might be more efficient impl. maybe erasing 1s in diff... but dunno
	uint16_t diff, new_mask;
	
	diff = ~( 
			(tern1.value & tern1.mask)	
			^
			(tern2.value & tern2.mask)
			);
	//erase right 1.
	for(new_mask=0xFFFF;new_mask;new_mask=new_mask<<1)
		if((diff&new_mask) == new_mask) break; 
	
	if(tern1.mask<new_mask || tern2.mask < new_mask )
		return NULL;
	
	if(new_mask)
		return __init_utern16(tern1.value,new_mask);
	
	return NULL;

}
inline utern_t* __utern32_get_alike(const utern32_t tern1, const utern32_t tern2){
	//TODO: there might be more efficient impl. maybe erasing 1s in diff... but dunno
	uint32_t diff, new_mask;

	diff = ~( 
			(tern1.value & tern1.mask)	
			^
			(tern2.value & tern2.mask)
			);
	//erase right 1.
	for(new_mask=0xFFFFFFFF;new_mask;new_mask=new_mask<<1)
		if((diff&new_mask) == new_mask) break; 
	
	if(tern1.mask<new_mask || tern2.mask < new_mask )
		return NULL;
	
	if(new_mask)
		return __init_utern32(tern1.value,new_mask);
	
	return NULL;

}
inline utern_t* __utern64_get_alike(const utern64_t tern1, const utern64_t tern2){
	//TODO: there might be more efficient impl. maybe erasing 1s in diff... but dunno
	uint64_t diff, new_mask;

	diff = ~( 
			(tern1.value & tern1.mask)	
			^
			(tern2.value & tern2.mask)
			);
	//erase right 1.
	for(new_mask=0xFFFFFFFFFFFFFFFF;new_mask;new_mask=new_mask<<1)
		if((diff&new_mask) == new_mask) break; 
	
	//FIXME assert unlikely
	//FIXME this condition happens also when two values are different in the non masked part.
		//in this case we sould return the utern as it is now.
	if(tern1.mask<new_mask || tern2.mask < new_mask )
		return NULL;
	
	if(new_mask)
		return __init_utern64(tern1.value,new_mask);
	
	return NULL;
}
inline utern_t* __utern128_get_alike(utern128_t tern1, utern128_t tern2){
	//TODO: there might be more efficient impl. maybe erasing 1s in diff... but dunno
	uint64_t diff_l,diff_h,new_mask;
	uint64_t complete_mask[2];

	diff_l = ~(	(tern1.value[UTERN128_LOW] & tern1.mask[UTERN128_LOW])	^	(tern2.value[UTERN128_LOW] & tern2.mask[UTERN128_LOW])	);
	diff_h = ~(	(tern1.value[UTERN128_HIGH] & tern1.mask[UTERN128_HIGH])	^	(tern2.value[UTERN128_HIGH] & tern2.mask[UTERN128_HIGH])	);
	
	//We first look for the common mask in the lower part
	for(new_mask=0xFFFFFFFFFFFFFFFF;new_mask;new_mask=new_mask<<1)
		if((diff_l&new_mask) == new_mask) break; 
	
	if( (tern1.mask[UTERN128_LOW]<new_mask || tern2.mask[UTERN128_LOW] < new_mask) && diff_h == 0xffffffffffffffff )
		return NULL;
		
	if(new_mask && diff_h == 0xffffffffffffffff){
		complete_mask[UTERN128_HIGH] = 0xffffffffffffffff;
		complete_mask[UTERN128_LOW] = new_mask;
		return __init_utern128(tern1.value,complete_mask);
	}
	
	//Now we look for it in the higher part
	for(new_mask=0xFFFFFFFFFFFFFFFF;new_mask;new_mask=new_mask<<1)
		if((diff_h&new_mask) == new_mask) break;
		
	if(tern1.mask[UTERN128_HIGH]<new_mask || tern2.mask[UTERN128_HIGH] < new_mask )
		return NULL;
	
	if(new_mask){
		complete_mask[UTERN128_HIGH]=new_mask;
		complete_mask[UTERN128_LOW]=0x0000000000000000;
		return __init_utern128(tern1.value,complete_mask);
	}
	
	return NULL;
}

#if 0
//Not used yet
inline bool __utern_compare128(const utern_t* tern, const void* value){
	utern128_t* tmp = (utern128_t*)tern;
	uint64_t* tmp_tern1 = (uint64_t*)tmp->value;
	uint64_t* tmp_tern2 = (uint64_t*)(tmp->value+64);
	uint64_t* tmp_mask1 = (uint64_t*)tmp->mask;
	uint64_t* tmp_mask2 = (uint64_t*)(tmp->mask+64);

	uint64_t* tmp_value1 = (uint64_t*)value;
	uint64_t* tmp_value2 = (uint64_t*)(value+64);
	
	return ((*tmp_tern1&*tmp_mask1) == (*tmp_value1&*tmp_mask1))
		&&
		((*tmp_tern2&*tmp_mask2) == (*tmp_value2&*tmp_mask2));
}
#endif

#if 0
///SLOW

/* Exported method utern_compare */ 
inline bool __utern_compare(const utern_t* tern, void* value){
	
	//TODO: tweak order for performance
	
	//Order is in purpose, not using switch too ;)
	if(tern->type == UTERN16_T){
		return utern_compare16(tern,value); 
	}else if(tern->type == UTERN32_T){
		return utern_compare32(tern,value); 
	}else if(tern->type == UTERN64_T){
		return utern_compare64(tern,value); 
	}else if(tern->type == UTERN8_T){
		return utern_compare8(tern,value); 
	}else if(tern->type == UTERN128_T){
		return utern_compare128(tern,value); 
	}else 
		return false;
}*/
#endif

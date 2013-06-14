#include <stdio.h>
#include <assert.h>
#include "CUnit/Basic.h"
#include "output_actions.h"

of12_switch_t *sw=NULL;
unsigned int port=2;
of12_flow_entry_t* entry=NULL;
of12_action_group_t *apply_actions = NULL; 
of12_write_actions_t *write_actions = NULL;
unsigned int grp_id=1;

int oa_set_up(void){
	
	physical_switch_init();	

	enum of12_matching_algorithm_available ma_list=of12_matching_algorithm_loop;
	sw = of12_init_switch("Test switch",0x0101,1,&ma_list);
	fprintf(stderr,"<%s:%d>sw %p\n",__func__,__LINE__,sw);
	
	entry = of12_init_flow_entry(NULL,NULL,false);
	apply_actions = of12_init_action_group(0);
	write_actions = of12_init_write_actions();
	assert (sw && entry && write_actions && apply_actions); //NOTE CU_ASSERT(sw!=0);
	return ROFL_SUCCESS;
}


int oa_tear_down(void){
	__of12_destroy_switch(sw);
	fprintf(stderr,"<%s:%d>\n",__func__,__LINE__);
	return ROFL_SUCCESS;
}


void oa_basic_test(void){
	/*
	 * This test needs to create a flow entry with different OUTPUT actions and check that the number is correct
	 * specifically we have to put OUTPUT_ACTIONS in the apply_actions, write_actions and in the groups.
	 */
	
	oa_set_up();
	
	//"map_flow_entry_actions"
	of12_packet_action_t* action = of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL);
	of12_push_packet_action_to_group(apply_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	//of12_packet_action_t* action = of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL);
	of12_set_packet_action_on_write_actions(write_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==1);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==1);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
	
}

void oa_only_apply(void){
	/*
	 * This test needs to create a flow entry with different OUTPUT actions and check that the number is correct
	 * specifically we have to put OUTPUT_ACTIONS in the apply_actions, write_actions and in the groups.
	 */
	oa_set_up();
	
	//"map_flow_entry_actions"
	of12_packet_action_t* action = of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL);
	of12_push_packet_action_to_group(apply_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	action = of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL);
	of12_set_packet_action_on_write_actions(write_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==1);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==false);
	
	oa_tear_down();
}

void oa_only_write(void){
	/*
	 * This test needs to create a flow entry with different OUTPUT actions and check that the number is correct
	 * specifically we have to put OUTPUT_ACTIONS in the apply_actions, write_actions and in the groups.
	 */
	oa_set_up();
	
	//"map_flow_entry_actions"
	of12_packet_action_t* action = of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL);
	of12_push_packet_action_to_group(apply_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	action = of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL);
	of12_set_packet_action_on_write_actions(write_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==1);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==false);
	
	oa_tear_down();
}

void oa_no_output(){
	
	oa_set_up();
	
	of12_packet_action_t* action = of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL);
	of12_push_packet_action_to_group(apply_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	//of12_packet_action_t* action = of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL);
	of12_set_packet_action_on_write_actions(write_actions,action);
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==false);
	
	oa_tear_down();
}

void oa_test_with_groups(void){
	//TODO add more output actions and GROUPS with output actions
	fprintf(stderr,"<%s:%d>\n",__func__,__LINE__);
	oa_set_up();

	of12_action_group_t *ag=of12_init_action_group(0);
	of12_action_group_t *ag2=of12_init_action_group(0);
	of12_bucket_list_t *buckets=of12_init_bucket_list();
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag2,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag2));
	of12_group_add(sw->pipeline->groups,OF12_GROUP_TYPE_ALL,grp_id,buckets);
	
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==3);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
}

void oa_two_outputs_apply(void){
	oa_set_up();
	
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port+1),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==2);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
	
}

void oa_two_outputs_write(void){
	fprintf(stderr,"<%s:%d>\n",__func__,__LINE__);
	oa_set_up();

	of12_action_group_t *ag=of12_init_action_group(0);
	of12_action_group_t *ag2=of12_init_action_group(0);
	of12_bucket_list_t *buckets=of12_init_bucket_list();
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag2,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag2));
	of12_group_add(sw->pipeline->groups,OF12_GROUP_TYPE_ALL,grp_id,buckets);
	
	//of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==2);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
}

void oa_write_and_group(void){
	fprintf(stderr,"<%s:%d>\n",__func__,__LINE__);
	oa_set_up();

	of12_action_group_t *ag=of12_init_action_group(0);
	of12_bucket_list_t *buckets=of12_init_bucket_list();
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag));
	of12_group_add(sw->pipeline->groups,OF12_GROUP_TYPE_ALL,grp_id,buckets);
	
	//of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==2);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
}

void oa_apply_and_group(void){
	fprintf(stderr,"<%s:%d>\n",__func__,__LINE__);
	oa_set_up();

	of12_action_group_t *ag=of12_init_action_group(0);
	of12_bucket_list_t *buckets=of12_init_bucket_list();
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_push_packet_action_to_group(ag,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_insert_bucket_in_list(buckets,of12_init_bucket(0,1,0,ag));
	of12_group_add(sw->pipeline->groups,OF12_GROUP_TYPE_ALL,grp_id,buckets);
	
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_GROUP,grp_id,NULL,NULL));
	of12_push_packet_action_to_group(apply_actions,of12_init_packet_action(OF12_AT_OUTPUT,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_APPLY_ACTIONS,apply_actions,NULL,0);
	
	//"map_flow_entry_actions"
	of12_set_packet_action_on_write_actions(write_actions,of12_init_packet_action(OF12_AT_DEC_NW_TTL,be32toh(port),NULL,NULL));
	of12_add_instruction_to_group(&entry->inst_grp,OF12_IT_WRITE_ACTIONS,NULL,write_actions,0);
	
	//insert flow entry
	of12_add_flow_entry_table(sw->pipeline, 0, entry, false, false);
	
	//check that the number of outputs is correct and the flag of multiple outpus is correctly set
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[0].apply_actions->num_of_output_actions==2);
	assert(sw->pipeline->tables[0].entries->inst_grp.instructions[2].write_actions->num_of_output_actions==0);
	assert(sw->pipeline->tables[0].entries->inst_grp.has_multiple_outputs==true);
	
	oa_tear_down();
}

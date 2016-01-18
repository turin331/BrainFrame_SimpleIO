/**\file */
#ifndef SLIC_DECLARATIONS_infoli_H
#define SLIC_DECLARATIONS_infoli_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*----------------------------------------------------------------------------*/
/*--------------------------- Interface writeLMem ----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'writeLMem'.
 * 
 * \param [in] param_size Interface Parameter "size".
 * \param [in] param_start Interface Parameter "start".
 * \param [in] instream_fromcpu The stream should be of size (param_size * 4) bytes.
 */
void infoli_writeLMem(
	int64_t param_size,
	int64_t param_start,
	const float *instream_fromcpu);

/**
 * \brief Basic static non-blocking function for the interface 'writeLMem'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_size Interface Parameter "size".
 * \param [in] param_start Interface Parameter "start".
 * \param [in] instream_fromcpu The stream should be of size (param_size * 4) bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *infoli_writeLMem_nonblock(
	int64_t param_size,
	int64_t param_start,
	const float *instream_fromcpu);

/**
 * \brief Advanced static interface, structure for the engine interface 'writeLMem'
 * 
 */
typedef struct { 
	int64_t param_size; /**<  [in] Interface Parameter "size". */
	int64_t param_start; /**<  [in] Interface Parameter "start". */
	const float *instream_fromcpu; /**<  [in] The stream should be of size (param_size * 4) bytes. */
} infoli_writeLMem_actions_t;

/**
 * \brief Advanced static function for the interface 'writeLMem'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void infoli_writeLMem_run(
	max_engine_t *engine,
	infoli_writeLMem_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'writeLMem'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_writeLMem_run_nonblock(
	max_engine_t *engine,
	infoli_writeLMem_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'writeLMem'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void infoli_writeLMem_run_group(max_group_t *group, infoli_writeLMem_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'writeLMem'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_writeLMem_run_group_nonblock(max_group_t *group, infoli_writeLMem_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'writeLMem'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void infoli_writeLMem_run_array(max_engarray_t *engarray, infoli_writeLMem_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'writeLMem'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_writeLMem_run_array_nonblock(max_engarray_t *engarray, infoli_writeLMem_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* infoli_writeLMem_convert(max_file_t *maxfile, infoli_writeLMem_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*---------------------------- Interface readLMem ----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'readLMem'.
 * 
 * \param [in] param_size Interface Parameter "size".
 * \param [in] param_start Interface Parameter "start".
 * \param [out] outstream_tocpu The stream should be of size (param_size * 4) bytes.
 */
void infoli_readLMem(
	int64_t param_size,
	int64_t param_start,
	float *outstream_tocpu);

/**
 * \brief Basic static non-blocking function for the interface 'readLMem'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_size Interface Parameter "size".
 * \param [in] param_start Interface Parameter "start".
 * \param [out] outstream_tocpu The stream should be of size (param_size * 4) bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *infoli_readLMem_nonblock(
	int64_t param_size,
	int64_t param_start,
	float *outstream_tocpu);

/**
 * \brief Advanced static interface, structure for the engine interface 'readLMem'
 * 
 */
typedef struct { 
	int64_t param_size; /**<  [in] Interface Parameter "size". */
	int64_t param_start; /**<  [in] Interface Parameter "start". */
	float *outstream_tocpu; /**<  [out] The stream should be of size (param_size * 4) bytes. */
} infoli_readLMem_actions_t;

/**
 * \brief Advanced static function for the interface 'readLMem'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void infoli_readLMem_run(
	max_engine_t *engine,
	infoli_readLMem_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'readLMem'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_readLMem_run_nonblock(
	max_engine_t *engine,
	infoli_readLMem_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'readLMem'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void infoli_readLMem_run_group(max_group_t *group, infoli_readLMem_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'readLMem'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_readLMem_run_group_nonblock(max_group_t *group, infoli_readLMem_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'readLMem'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void infoli_readLMem_run_array(max_engarray_t *engarray, infoli_readLMem_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'readLMem'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_readLMem_run_array_nonblock(max_engarray_t *engarray, infoli_readLMem_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* infoli_readLMem_convert(max_file_t *maxfile, infoli_readLMem_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/



/**
 * \brief Auxiliary function to evaluate expression for "infoliKernel.writeLoop0".
 */
int infoli_get_infoliKernel_writeLoop0( void );


/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] param_Max_N_Size Interface Parameter "Max_N_Size".
 * \param [in] param_N Interface Parameter "N".
 * \param [in] param_N_size Interface Parameter "N_size".
 * \param [in] param_simsteps Interface Parameter "simsteps".
 * \param [in] param_time_mux_factor Interface Parameter "time_mux_factor".
 */
void infoli(
	int64_t param_Max_N_Size,
	int64_t param_N,
	int64_t param_N_size,
	int64_t param_simsteps,
	int64_t param_time_mux_factor);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_Max_N_Size Interface Parameter "Max_N_Size".
 * \param [in] param_N Interface Parameter "N".
 * \param [in] param_N_size Interface Parameter "N_size".
 * \param [in] param_simsteps Interface Parameter "simsteps".
 * \param [in] param_time_mux_factor Interface Parameter "time_mux_factor".
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *infoli_nonblock(
	int64_t param_Max_N_Size,
	int64_t param_N,
	int64_t param_N_size,
	int64_t param_simsteps,
	int64_t param_time_mux_factor);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	int64_t param_Max_N_Size; /**<  [in] Interface Parameter "Max_N_Size". */
	int64_t param_N; /**<  [in] Interface Parameter "N". */
	int64_t param_N_size; /**<  [in] Interface Parameter "N_size". */
	int64_t param_simsteps; /**<  [in] Interface Parameter "simsteps". */
	int64_t param_time_mux_factor; /**<  [in] Interface Parameter "time_mux_factor". */
} infoli_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void infoli_run(
	max_engine_t *engine,
	infoli_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'default'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_run_nonblock(
	max_engine_t *engine,
	infoli_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void infoli_run_group(max_group_t *group, infoli_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_run_group_nonblock(max_group_t *group, infoli_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void infoli_run_array(max_engarray_t *engarray, infoli_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *infoli_run_array_nonblock(max_engarray_t *engarray, infoli_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* infoli_convert(max_file_t *maxfile, infoli_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* infoli_init(void);

/* Error handling functions */
int infoli_has_errors(void);
const char* infoli_get_errors(void);
void infoli_clear_errors(void);
/* Free statically allocated maxfile data */
void infoli_free(void);
/* These are dummy functions for hardware builds. */
int infoli_simulator_start(void);
int infoli_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_infoli_H */


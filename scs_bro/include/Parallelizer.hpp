/*
 * =====================================================================================
 *
 *       Filename:  Parallelizer.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/19/2013 08:50:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chen Yuheng (Chen Yuheng), chyh1990@163.com
 *   Organization:  Tsinghua Unv.
 *
 * =====================================================================================
 */

#ifndef __FW_PARALLELIZER_HPP
#define __FW_PARALLELIZER_HPP

#include "compiler.h"
#include <Thread.h>
namespace utils{
	namespace concurrency{

	class Job{
		public:
			typedef int (*job_t)(int, void *);
			job_t job;
			void *data;
			bool valid;
			Job(job_t _job, void *_data)
				:job(_job), data(_data),
				valid(false){
			}

			Job()
				:job(NULL), data(NULL),
				valid(false){}
	};

	class Parallelizer{
		private:
			static const int maxThreads = 16;

			int tids[maxThreads];
			pthread_t p_threads[maxThreads];

			int remain_thread;
			pthread_cond_t job_done_cond;
			pthread_mutex_t job_done_mutex;

			int threads;

			pthread_cond_t job_control_cond;
			pthread_mutex_t job_control_mutex;
			int take_job;
			bool workerKilled;

			Job job;

			struct WorkerParam{
				Parallelizer *thiz;
				int id;
			};
			WorkerParam params[maxThreads];

			static void *MainLoop(void *data){
				WorkerParam *param = (WorkerParam*)data;
				Parallelizer *thiz = param->thiz;
				int id = param->id;
				FW_ASSERT(id < thiz->threads);
				FW_DEBUG_TRACE("pid %d start\n", param->id);
				while(!thiz->workerKilled){
					int jobid = -1;
					pthread_mutex_lock(&thiz->job_control_mutex);
					while(thiz->take_job == 0){
						if(thiz->workerKilled)
							break;
						pthread_cond_wait(&thiz->job_control_cond, &thiz->job_control_mutex);
					}
					if(thiz->workerKilled){
						pthread_mutex_unlock(&thiz->job_control_mutex);
						break;
					}
					thiz->take_job --;
					jobid = thiz->take_job;
					pthread_mutex_unlock(&thiz->job_control_mutex);

					FW_ASSERT(thiz->job.valid);
					//DO JOB
					FW_DEBUG_TRACE("%d HERE jobid %d\n", id, jobid);
					if(thiz->job.job)
						(thiz->job.job)(jobid, thiz->job.data);


					pthread_mutex_lock(&thiz->job_done_mutex);
					thiz->remain_thread --;
					if(thiz->remain_thread == 0)
						pthread_cond_signal(&thiz->job_done_cond);
					pthread_mutex_unlock(&thiz->job_done_mutex);

				}
				FW_DEBUG_TRACE("pid %d done", param->id);
				return NULL;
			}

		public:
			Parallelizer(int _threads){
				threads = _threads;
				if(threads > maxThreads)
					threads = maxThreads;
				workerKilled = false;
				pthread_mutex_init(&job_done_mutex, NULL);
				pthread_mutex_init(&job_control_mutex, NULL);
				pthread_cond_init(&job_control_cond, NULL);
				pthread_cond_init(&job_done_cond, NULL);
			}

			inline int GetThreads() const{
				return threads;
			}

			void InitPool(){
				take_job = 0;
				for(int i=0;i<threads;i++){
					params[i].thiz = this;
					params[i].id = i;
					tids[i] = pthread_create(&p_threads[i], NULL, Parallelizer::MainLoop, (void*)&params[i]);
				}
				FW_DEBUG_TRACE("Pool inited");
			}

			void Work(const Job &_job){
				job = _job;
				FW_ASSERT(_job.valid == false);
				job.valid = true;
				remain_thread = threads;

				pthread_mutex_lock(&job_control_mutex);
				take_job = threads;
				pthread_cond_broadcast(&job_control_cond);
				pthread_mutex_unlock(&job_control_mutex);

				pthread_mutex_lock(&job_done_mutex);
				while(remain_thread != 0)
					pthread_cond_wait(&job_done_cond, &job_done_mutex);
				pthread_mutex_unlock(&job_done_mutex);
				FW_DEBUG_TRACE("Work done");
			}

			void ReleasePool(){
				void *dummy;
				FW_DEBUG_TRACE("Killing");
				pthread_mutex_lock(&job_control_mutex);
				workerKilled = true;
				pthread_cond_broadcast(&job_control_cond);
				pthread_mutex_unlock(&job_control_mutex);
				for(int i=0;i<threads;i++)
					pthread_join(p_threads[i], &dummy);
				pthread_mutex_destroy(&job_done_mutex);
				pthread_mutex_destroy(&job_control_mutex);
				pthread_cond_destroy(&job_control_cond);
				pthread_cond_destroy(&job_done_cond);
				FW_DEBUG_TRACE("Pool Killed");
			}
	};

	}
}

#endif

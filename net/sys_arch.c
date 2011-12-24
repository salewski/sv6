#include "lwip/sys.h"
#include "arch/sys_arch.h"

#include "queue.h"
#include "kernel.h"
#include "proc.h"

#define DIE panic(__func__)

//
// mbox
//
err_t
sys_mbox_new(sys_mbox_t *mbox, int size)
{
  if (size > MBOXSLOTS) {
    cprintf("sys_mbox_new: size %u\n", size);
    return ERR_MEM;
  }
  mbox->head = 0;
  mbox->tail = 0;
  initlock(&mbox->s, "lwIP mbox");
  initcondvar(&mbox->c, "lwIP mbox");

  return ERR_OK;
}

void
sys_mbox_set_invalid(sys_mbox_t *mbox)
{
  DIE;
}

err_t
sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  DIE;
}

int
sys_mbox_valid(sys_mbox_t *mbox)
{
  DIE;
}

void
sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  DIE;
}

void
sys_mbox_free(sys_mbox_t *mbox)
{
  DIE;
}

u32_t
sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
  u64 start, to;
  u32 r;

  acquire(&mbox->s);
  start = nsectime();
  to = (u64)timeout*1000000 + start;
  while (mbox->head-mbox->tail == 0) {
    if (to < nsectime()) {
      r = SYS_ARCH_TIMEOUT;
      goto done;
    }
    cv_sleepto(&mbox->c, &mbox->s, to);
  }
  r = nsectime()-start;
  *msg = mbox->msg[mbox->tail % MBOXSLOTS];
  mbox->tail++;

done:
  release(&mbox->s);
  return r;
}

u32_t
sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  DIE;
}

//
// sem
//
err_t
sys_sem_new(sys_sem_t *sem, u8_t count)
{
  initlock(&sem->s, "lwIP sem");
  initcondvar(&sem->c, "lwIP condvar");
  sem->count = count;
  return ERR_OK;
}

void
sys_sem_free(sys_sem_t *sem)
{
  DIE;
}

void
sys_sem_set_invalid(sys_sem_t *sem)
{
  DIE;
}

int
sys_sem_valid(sys_sem_t *sem)
{
  DIE;
}

void
sys_sem_signal(sys_sem_t *sem)
{
  DIE;
}

u32_t
sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  DIE;
}

//
// mutex
//
err_t
sys_mutex_new(sys_mutex_t *mutex)
{
  initlock(&mutex->s, "lwIP mutex");
  return ERR_OK;
}

void
sys_mutex_lock(sys_mutex_t *mutex)
{
  DIE;
}

void
sys_mutex_unlock(sys_mutex_t *mutex)
{
  DIE;
}

void
sys_mutex_free(sys_mutex_t *mutex)
{
  DIE;
}

//
// thread
//
sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn thread, void *arg,
               int stacksize, int prio)
{
  struct proc *p;
  
  p = threadalloc(thread, arg);
  safestrcpy(p->name, name, sizeof(p->name));

  acquire(&p->lock);
  p->state = RUNNABLE;
  addrun(p);
  release(&p->lock);

  return p;
}

//
// init
//
void
sys_init(void)
{

}

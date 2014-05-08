from donut import cruller, bearclaw
import multiprocessing as mp


nProc = mp.cpu_count()

def thread(inList, func):
    pool = mp.Pool(processes=nProc)
    
    outList = pool.map(func, inList)
    pool.close()
    pool.join()

    return outList


#!/usr/bin/env python
from __future__ import print_function
import timeit

PAR_EXEC   = '../build/mandelbrot_parallel'
SEQ_EXEC   = '../build/mandelbrot'
NTHREADS   = range(1, 9, 2)
NRUNS      = 10
ROWS       = 10000
COLUMNS    = 10000
ITERATIONS = 1000

if __name__ == '__main__':
    times = []
    speedups = []
    efficiencies = []

    seq_cmd = """
    subprocess.call(
        ['{}', '{}'],
        stdout=open(os.devnull, 'w')
    )
    """

    par_cmd = """
    subprocess.call(
        ['{}', '{}', '{}'],
        stdout=open(os.devnull, 'w')
    )
    """

    for num_points in NPOINTS:
        print('num points = ', num_points, '\n')
        ts, ss, efs = [], [], []
        for num_thread in NTHREADS:
            seq_time = timeit.timeit(
                stmt=seq_cmd.format(SEQ_EXEC, num_points),
                setup="import subprocess, os",
                number=NRUNS
            ) / NRUNS

            avg_time = timeit.timeit(
                stmt=par_cmd.format(PAR_EXEC, num_points, num_thread),
                setup="import subprocess, os",
                number=NRUNS
            ) / NRUNS

            speedup = seq_time / avg_time
            efficiency = speedup / num_thread
            times.append(avg_time)
            speedups.append(speedup)
            efficiencies.append(efficiency)
            print(','.join(map(str, [seq_time, avg_time,speedup,efficiency])))
        print()

    try:
        import matplotlib.pyplot as plt

        def generate_plot(x, y, title, xlabel, ylabel, fname):
            plt.plot(x, y)
            plt.title(title)
            plt.xticks(NTHREADS)
            plt.xlabel(xlabel)
            plt.ylabel(ylabel)
            plt.minorticks_on()
            plt.grid(which='major')
            plt.savefig(fname)
            plt.clf()
        """
        for (i, (thread, points)) in thread_point:
            generate_plot(
                x=NTHREADS,
                y=times,
                title='Number of Threads vs Time',
                xlabel='Number of Threads',
                ylabel='Time (seconds)',
                fname='thread_vs_time.png'
            )
            generate_plot(
                x=NTHREADS,
                y=speedups,
                title='Number of Threads vs Speedup',
                xlabel='Number of Threads',
                ylabel='Speedup',
                fname='thread_vs_speedup.png'
            )
            generate_plot(
                x=NTHREADS,
                y=efficiencies,
                title='Number of Threads vs Efficiency',
                xlabel='Number of Threads',
                ylabel='Efficiency',
                fname='thread_vs_efficiency.png'
            )
        """
    except Exception as err:
        print(err)


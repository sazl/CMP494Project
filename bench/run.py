#!/usr/bin/env python
from __future__ import print_function
import timeit

SEQ_EXEC   = '../bin/mandelbrot'
PAR_EXEC   = '../bin/mandelbrot_parallel'
OUTPUT_DIR = '../output/'
NRUNS      = 5
WIDTHS     = [2 ** x for x in range(9, 14)]
ITERATIONS = [2 ** x for x in range(9, 14, 2)]
BLOCK_DIM  = [2 ** x for x in range(4, 10)]

if __name__ == '__main__':
    times = []
    speedups = []
    efficiencies = []

    seq_cmd = """
    subprocess.call(
        ['{}', '{}', '{}', '{}', '{}'],
        stdout=open(os.devnull, 'w')
    )
    """

    par_cmd = """
    subprocess.call(
        ['{}', '{}', '{}', '{}', '{}', '{}', '{}'],
        stdout=open(os.devnull, 'w')
    )
    """

    for num_iterations in ITERATIONS:
        print('num iterations = ', num_iterations, '\n')
        ts, ss, efs = [], [], []
        for width in WIDTHS:
            height = width
            print('width = {: <6} height = {: <6} '.format(width, height))
            seq_time = timeit.timeit(
                stmt=seq_cmd.format(SEQ_EXEC, width, height, num_iterations,
                                    OUTPUT_DIR + '_'.join(map(str, [
                                        width, height, num_iterations, 'seq.png']))),
                setup="import subprocess, os",
                number=NRUNS
            ) / NRUNS

            for dim in BLOCK_DIM:
                block_dim_x, block_dim_y = dim, dim
                grid_dim_x, grid_dim_y = width/block_dim_x, height/block_dim_y
                num_thread = block_dim_x * block_dim_y * grid_dim_x * grid_dim_y
                print(('block_dim_x = {: <5} block_dim_y = {: <5}' +
                       ' grid_dim_x = {: <5} grid_dim_y = {: <5}'  +
                       ' num_threads = {: <10}').format(
                           block_dim_x, block_dim_y,
                           grid_dim_x, grid_dim_y,
                           num_thread
                       ))
                
                par_time = timeit.timeit(
                    stmt=par_cmd.format(PAR_EXEC, width, height, num_iterations,
                                        block_dim_x, block_dim_y,
                                        OUTPUT_DIR + '_'.join(map(str, [
                                            width, height, num_iterations,
                                            block_dim_x, block_dim_y, 'par.png'
                                        ]))),
                    setup="import subprocess, os",
                    number=NRUNS
                ) / NRUNS

                speedup = seq_time / par_time
                efficiency = speedup / num_thread
                times.append(par_time)
                speedups.append(speedup)
                efficiencies.append(efficiency)
                print('{: <15} {: <15} {: <15} {: <15}'.format(
                    seq_time, par_time, speedup, efficiency))
            print()
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


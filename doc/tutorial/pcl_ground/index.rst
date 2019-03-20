.. _pcl_ground:

===============================================================================
Identifying ground returns
===============================================================================

One of the most commonly used tasks within the PDAL application is that of
segmenting ground returns in point cloud datasets. The goal of this tutorial is
to share several of the options within PDAL for completing this task.

To run the examples in this tutorial, download the dataset `CSite1_orig-utm.laz
<https://raw.github.com/PDAL/data/master/isprs/CSite1_orig-utm.laz>`_ and save
it to disk.

.. image:: csite-original.png
   :height: 400px

Simple Morphological Filter
------------------------------------------------------------------------------

SMRF [Pingel2013]_.

filters.smrf :ref:`smrf <filters.smrf>`

In each of the examples in this section, we will call the following ``pdal translate`` command. Simply create the ``pipeline.json`` according to each example and update the path as necessary.

::

    $ pdal translate -i /path/to/CSite1_orig-utm.laz \
        -o /path/to/output.laz \
        --json /path/to/pipeline.json

.. literalinclude:: smrf-range.json

.. image:: csite-smrf-default.png
   :height: 400px

.. literalinclude:: outlier-smrf-range.json

.. image:: csite-smrf-denoise.png
   :height: 400px

.. literalinclude:: outlier-smrf-range-cut.json

.. image:: csite-smrf-cut.png
   :height: 400px


Progressive Morphological Filter
------------------------------------------------------------------------------

PMF [Zhang2003]_.

filters.pmf :ref:`pmf <filters.pmf>`

pdal ground -i ~/Data/bare_earth_eval/isprs/converted/laz/CSite1_orig-utm.laz -o ~/Temp/ground-kernel-defaults.laz -v4
pdal ground -i ~/Data/bare_earth_eval/isprs/converted/laz/CSite1_orig-utm.laz -o ~/Temp/ground-kernel-defaults.laz --verbose 4
pdal ground -i ~/Data/bare_earth_eval/isprs/converted/laz/CSite1_orig-utm.laz -o ~/Temp/ground-kernel-defaults.laz --extract --verbose 4
pdal ground -i ~/Data/bare_earth_eval/isprs/converted/laz/CSite1_orig-utm.laz -o ~/Temp/ground-kernel-denoise.laz --extract --denoise --verbose 4
pdal ground -i ~/Data/bare_earth_eval/isprs/converted/laz/CSite1_orig-utm.laz -o ~/Temp/ground-kernel-denoise-cell.laz --extract --denoise --cell_size=1.5 --verbose 4

A complete description of the algorithm can be found in the article `"A
Progressive Morphological Filter for Removing Nonground Measurements from
Airborne LIDAR Data" <http://users.cis.fiu.edu/~chens/PDF/TGRS.pdf>`_ by K.
Zhang, S. Chen, D. Whitman, M. Shyu, J. Yan, and C. Zhang.

We have chosen to demonstrate the algorithm using data from the 2003 report
"ISPRS Comparison of Filters." For more on the data and the study itself,
please see http://www.itc.nl/isprswgIII-3/filtertest/ as well as `"Experimental
comparison of filter algorithms for bare-earth extraction from airborne laser
scanning point clouds" <http://dx.doi.org/10.1016/j.isprsjprs.2004.05.004>`_ by
G. Sithole and G. Vosselman.

Using the Ground kernel
------------------------------------------------------------------------------

The :ref:`pdal ground <ground_command>` kernel can be used to filter ground
returns, allowing the user to tweak filtering parameters at the command-line.

Let's start by running ``pdal ground`` with the default parameters.

::

    $ pdal ground -i CSite1_orig-utm.laz -o CSite1_orig-utm-ground.laz

To get an idea of what's happening during each iteration, you can optionally
increase the verbosity of the output. We'll try ``-v4``.  Here we see a summary
of the parameters, along with height threshold, window size, and number of
remaining ground points.

::

    $ pdal ground -i CSite1_orig-utm.laz -o CSite1_orig-utm-ground.laz -v4
    
The resulting filtered cloud can be seen in this top-down and front view. When
viewed from the side, it is apparent that there are a number of low noise
points that have fooled the PMF filter.

.. image:: csite-pmf-default.png
   :height: 400px

.. image:: csite-pmf-front.png
   :height: 400px


To address, we introduce an alternate way to call PMF

The result is noticeably cleaner in both the top-down and front views.

.. image:: csite-pmf-denoise.png
   :height: 400px

.. image:: csite-denoised-front.png
   :height: 400px

Unfortunately, you may notice that we still have a rather large building in the
lower right of the image. By tweaking the parameters slightly, in this case,
increasing the cell size, we can do a better job of removing such features.

Once again, the result is noticeably cleaner.

.. image:: csite-pmf-cellsize.png
   :height: 400px


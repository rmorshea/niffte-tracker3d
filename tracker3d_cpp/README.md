###3-D Trajectory Generation in Hexagonal Geometry
#####author: Ryan Morshead  
Groups discrete data events into directional trajectories in a 3-D space.
Based on Neutron Induced Fission Fragment Tracking Experiment (NIFFTE) time projection chamber data.
This is C++ interpretation of:
https://github.com/rmorshea/niffte-tracker3d/blob/master/tracker3d_ipy/tracker3d.ipynb
for later integration into the ROOT framework implamented by NIFFTE.

####findTrajectories (as of 7/28/14)
The findTrajectories method is going to take some time to think through if we approach it from my method.
I keep thinking of little areas that will need to be addressed. To give a general outline though,
the idea would essentially be to develope networks of smaller "paths" that could give enough information
to identify a larger overall structure (i.e. the trajectories). Figuring out how the characteristics
of a path relate to that overal structure, either through their average ADC values, direction, or relationships
to other nearby paths, is something I'm still a unsure of.

My initial idea for creating these paths would be to pick arbitrary starting points (though it may be
beneficial to start with those that have the highest ADC value) and simply follow the path of least reistance
through neighboring voxels until there are no more neighbors, or until the least resistant paths includes
a voxel which is already used in another path.

Right off the bat though, this presents a problem because the length of the paths selected first, would be
unhelpfully long since the chances of running into another path or hitting a dead end are slim. In addition,
given a choice between two voxels this methodology would have us make a relative choice between them regardless
of whether either was actually a good choice. To solve this, one could implament a sort of point system in which
each decision to extend a path would cost points. resitantive choices would cost more, while conformative choices
would cost less. Thus the overall trend would be that less resistive paths would tend to be longer.

#####-- Benefits --
The cost of a deviation could be customized. For example, one could implanet a method in which cost increased
expentially with the degree of deviation, or where the cost model for different kinds of deviations could
differ; one could decide that local directional changes could be less costly than deviations from the average path
direction.

The amount of information that a network of these paths would provide may allow trajectories to be identified
more accurately. For instance, in the case where two trajectories intersect, the number of paths that
merge with, or lead to a particular voxel, local cluster of voxels, or particular path, could be indicative of a choke
point where two trajectories meet. On the other hand simply binning the average direction of each path may
indicate the presence of two prefered path directions.

#####-- Drawbacks --
In general this approach seems as if it could be quite complicated, and the ways that it could fail may
not necessarilly be predictable. In contrast, John's code appears to be fairly consitant in the way that it
both fails, and succeeds at identifying trajectories. It may be more worth while investigating Johns method for
this reason.

#####-- Radical Ideas --
An interesting avenue to explore is the fact that the paths that are generated will be different depending on
what voxels are used to start generating paths. Generating a few different initial starting conditions, analyzing
the results, picking the condition which produces results with the least noise over, and introducing slight
variations over the course of a few generations could be used to optimize the paths which are ultimately generated.  
(this might be relivant for getting the best spines in Johns code as well)

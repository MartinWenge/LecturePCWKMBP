### Lecture Material CSN WKMBP ###

This repo concerns the lecture
** Computersimulation und Numerik in der weichen kondensierten Materie**

There is also a [website](http://www.ipfdd.de/de/scmbp/soft-condensed-matter-and-biological-physics/numerik-und-computersimulationen-in-der-weichen-kondensierten-materie/)for this lecture.

Here you find the student templates and some of the solutions and presentation (jupyter) notebooks. 

To use the material simple type
```
git clone https://github.com/MartinWenge/LecturePCWKMBP.git
```
or - best pratice - use the [Forking Workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/forking-workflow) and clone your own fork to be able to create pull requests. 

Then you can upload your solutions to be checked using an intuitively named branch:

```
git checkout -b yourName_branch
# Edit some code
git commit -a -m "Add first draft of some feature"
# if main repo has moved forward and its remote was called upstream
git pull upstream master
# push changes to your fork and do the pull request on github
git push origin yourName_branch

```
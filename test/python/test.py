#!/usr/bin/python

from satlib import *
import pprint
print 'Creating solver'
Solver = CreateSolver(SOLVER_MINISAT)
print 'Pushing'
Solver.Push()
AExp = Solver.CreateVar('AA')
print 'Creating exp'
NotAExp = Solver.CreateNot(AExp)
print 'Creating or'
AAndNotAExp = Solver.CreateBOr(AExp, NotAExp)
Solver.Assert(AExp)
print 'Solving'
Res = Solver.Solve()
print Res
print 'Getting model'
Model = Solver.GetModel();
for (key, value) in Model.iteritems():
    if value == VAR_TRUE:
        print '%s = true' % key
    else:
        print '%s = false' % key
print 'Model complete'

Solver.Assert(NotAExp)
Res = Solver.Solve()
print Res
Model = Solver.GetModel();
for (key, value) in Model.iteritems():
    if value == VAR_TRUE:
        print '%s = true' % key
    else:
        print '%s = false' % key



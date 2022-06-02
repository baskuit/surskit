#pragma once

#include <string.h>
#include <iostream>

#include "../libsurskit/math.hh"


    // Data

    // StateInfo


struct StateInfo {};

struct SolvedStateInfo : StateInfo {

    bool terminal;
    int rows;
    int cols;
    float* strategy0;
    float* strategy1;
    float payoff;
    
    // iirc, If I remove the empty constructor, I get errors?
    SolvedStateInfo ();
    SolvedStateInfo (int rows, int cols, float payoff) :
    terminal(rows * cols == 0), rows(rows), cols(cols), strategy0(new float[rows]), strategy1(new float[cols]), payoff(payoff) {};
    SolvedStateInfo (int rows, int cols, float* strategy0, float* strategy1, float payoff) :
    terminal(rows * cols == 0), rows(rows), cols(cols), strategy0(strategy0), strategy1(strategy1), payoff(payoff) {};

    // shallow copy will not work since we modify the strategies during transition
    SolvedStateInfo (SolvedStateInfo const& info) {
        terminal = info.terminal;
        rows = info.rows;
        cols = info.cols;
        payoff = info.payoff;
        strategy0 = new float[rows];
        strategy1 = new float[cols];
        memcpy(strategy0, info.strategy0, rows*sizeof(float)); 
        memcpy(strategy1, info.strategy1, cols*sizeof(float));
    };

    ~SolvedStateInfo () {
        delete [] strategy0;
        delete [] strategy1;
    }
};


    // State


class State {
public:

    StateInfo* info; // default is now deep copy
    prng device; //default copy copies the progress
    // thus state default copy is deep copy

    State () {};
    State (StateInfo* info, prng device) : info(info), device(device) {};
    State (StateInfo* info) : info(info) {}; // TODO does prng device; initilialize?

    State* copy () {
        State* x = new State(info, device.copy());
        return x;
    }

    virtual PairActions actions () {
        return PairActions();
    }
    virtual void actions (PairActions actions) {}
    virtual StateTransitionData transition(Action action0, Action action1) {return StateTransitionData();};
    virtual float rollout() {return 0.5f;};

};
#include "../../include/RubiksCube/Solver.h"
#include <ctime>

typedef RubiksCube::EDGE EDGE;
typedef RubiksCube::CORNER CORNER;
typedef RubiksCube::MOVE MOVE;

Solver::Solver()
{
    startNode.cube = RubiksCube();
    startNode.path = "";
    startNode.lastMove = MOVE::NONE;
    totalSteps = 0;

    for (int i = 0; i < 18; i++)
        allowed[(MOVE)i] = true;
    aux.scramble(startNode.cube, 100);
}

Solver::Solver(RubiksCube &cube)
{
    startNode.cube = cube;
    startNode.path = "";
    startNode.lastMove = MOVE::NONE;
    totalSteps = 0;
    for (int i = 0; i < 18; i++)
        allowed[(MOVE)i] = true;
}

void Solver::solveCube()
{
    solutionMoves.clear();
    for (int i = 0; i < 4; i++)
    {
        PHASE phase = (PHASE)i;
        bidirectionalBFS(phase);
        removeMoves(phase);
    }

    totalSteps = 0;
}

void Solver::removeMoves(PHASE phase)
{
    typedef MOVE MOVE;
    switch (phase)
    {
    case PHASE::PHASE1:
        allowed[MOVE::F] = false;
        allowed[MOVE::FPRIME] = false;
        allowed[MOVE::B] = false;
        allowed[MOVE::BPRIME] = false;
        break;
    case PHASE::PHASE2:
        allowed[MOVE::L] = false;
        allowed[MOVE::LPRIME] = false;
        allowed[MOVE::R] = false;
        allowed[MOVE::RPRIME] = false;
        break;
    case PHASE::PHASE3:
        allowed[MOVE::U] = false;
        allowed[MOVE::UPRIME] = false;
        allowed[MOVE::D] = false;
        allowed[MOVE::DPRIME] = false;
        break;
    default:
        break;
    }
}

// BFS to solve the cube for testing...
void Solver::bidirectionalBFS(Solver::PHASE phase)
{
    // typedef MOVE MOVE;

    MOVE moves[18] = {
        MOVE::L,
        MOVE::LPRIME,
        MOVE::L2,
        MOVE::R,
        MOVE::RPRIME,
        MOVE::R2,
        MOVE::U,
        MOVE::UPRIME,
        MOVE::U2,
        MOVE::D,
        MOVE::DPRIME,
        MOVE::D2,
        MOVE::F,
        MOVE::FPRIME,
        MOVE::F2,
        MOVE::B,
        MOVE::BPRIME,
        MOVE::B2,
    };
    map<state, short> dir;
    map<state, MOVE> lastMove;
    map<state, state> parent;

    RubiksCube solvedCube;
    state currentId = getPhaseId(phase, startNode.cube);
    state goalId = getPhaseId(phase, solvedCube);

    dir[currentId] = 1, dir[goalId] = 2;

    // if the goal is already achieved
    if (currentId == goalId)
        return;

    queue<RubiksCube> q;
    q.push(startNode.cube);
    q.push(solvedCube);

    lastMove[currentId] = startNode.lastMove; // Keep the last move from the last phase
    bool isSolved = false;
    vector<MOVE> phaseMoves;

    while (!q.empty() && !isSolved)
    {
        RubiksCube topNode = q.front();
        state topId = getPhaseId(phase, topNode);
        auto topDir = dir[topId];
        q.pop();
        for (int i = 0; i < 18; i++)
        {
            MOVE move = moves[i];
            if (!allowed[move] || prune(lastMove[topId], move))
                continue;
            topNode.move(move);
            state newId = getPhaseId(phase, topNode);
            auto newDir = dir[newId];

            if (newDir && newDir != topDir)
            {
                // You found it. Reconstruct the Path!
                // cout << "Goal achivied\n";
                if (topDir == 2)
                {
                    // if you come from right swap the id to construct the algorithm from left to right
                    swap(topId, newId);
                    move = RubiksCube::invert(move);
                }
                while (topId != currentId)
                {
                    phaseMoves.insert(phaseMoves.begin(), lastMove[topId]);
                    topId = parent[topId];
                }
                phaseMoves.push_back(move);

                while (newId != goalId)
                {
                    phaseMoves.push_back(RubiksCube::invert(lastMove[newId]));
                    newId = parent[newId];
                }
                isSolved = true;
                break;
            }
            if (!newDir)
            {
                // add the new node to the queue
                q.push(topNode);
                dir[newId] = topDir;
                lastMove[newId] = move;
                parent[newId] = topId;
            }
            topNode.move(RubiksCube::invert(move));
        }
    }

    if (isSolved)
    {
        cout << "Steps: ";
        totalSteps += (int)phaseMoves.size();
        for (auto move : phaseMoves)
        {
            solutionMoves.push_back(move);
            startNode.cube.move(move);
            startNode.path += aux.convertMove(move) + char(' ');
            cout << aux.convertMove(move) << " ";
        }
        cout << "\n";
        startNode.lastMove = phaseMoves[phaseMoves.size() - 1];
        if (goal(phase, startNode.cube))
        {
            if (phase == PHASE::PHASE1)
            {
                cout << "Phase 1 Completed with " << phaseMoves.size() << " moves \nAll the edges are correctly oriented.\nNow the cube can be solved with <U,B,L,R,F2,B2> moves\n\n";
            }
            else if (phase == PHASE::PHASE2)
            {
                cout << "Phase 2 Completed with " << phaseMoves.size() << " moves \nAll the corners are correctly oriented and the edges of the E-Slice are positioned.\nNow the cube can be solved with <U,B,L2,R2,F2,B2> moves\n\n";
            }
            else if (phase == PHASE::PHASE3)
            {
                cout << "Phase 3 Completed with " << phaseMoves.size() << " moves \nAll the corners are on their correct orbit and the edges of the M,S-Slice are positioned.\nNow the cube can be solved with only half twists <U2,B2,L2,R2,F2,B2> moves\n\n";
            }
            else if (phase == PHASE::PHASE4)
            {

                cout << "Phase 4 Completed with " << phaseMoves.size() << " moves\nRubiks Cube SOLVED! :)\n"
                                                                          "Complete Steps: "
                     << startNode.path << "\n"
                     << "Solved in : " << totalSteps << " steps \n";
            }
        }
        else
        {
            cout << "\n\nThere was an error and the Cube cannot be solved :(\n\n";
            throw "\n\nThere was an error and the Cube cannot be solved :(\n\n";
        }
    }
}

Solver::state Solver::getPhaseId(PHASE phase, RubiksCube &cube)
{

    state st;
    if (phase == PHASE1)
    {
        // track the edges orientation (the index does not matter)
        st.assign(1, 0);
        for (int i = 0; i < 12; i++)
        {
            if (cube.edges[i].orientation == 0)
                st[0] |= (1 << (i));
        }
    }
    else if (phase == PHASE2)
    {
        // Check the corner orientation and the correct position of the E-Slice
        st.assign(2, 0);
        for (int i = 0; i < 8; i++)
        {
            if (cube.corners[i].orientation != 0)
                st[0] |= (1 << (i * 2));
            if (cube.corners[i].orientation == 2)
                st[0] |= (1 << (i * 2 + 1));
        }
        for (int i = 0; i < 12; i++)
        {
            EDGE edge = (EDGE)cube.edges[i].index;
            if (edge == EDGE::LF || edge == EDGE::RF || edge == EDGE::LB || edge == EDGE::RB)
                st[1] |= (1 << i);
        }
    }
    else if (phase == PHASE3)
    {
        // Check for the correct orbits of the corners, the S,M-Slice  and the overall parity
        // if the E,M-Slice is corrected then the S-Slice too
        st.assign(6, 0);
        // Check if the M-Slice is correct
        for (int i = 0; i < 12; i++)
        {
            EDGE edge = (EDGE)cube.edges[i].index;
            if (edge == EDGE::UF || edge == EDGE::DF || edge == EDGE::UB || edge == EDGE::DB)
                st[0] |= (1 << i);
        }
        /*
        Check if the pairs of corners {URF,ULB} {DLF,DRB} {ULF,URB} {DRF,DLB} are in their correct position:
        Ex: URF is on URF or ULB, DLF is on DLF or DRB...
        */
        for (int i = 0; i < 8; i++)
        {
            CORNER corner = (CORNER)cube.corners[i].index;
            if (corner == CORNER::URF || corner == CORNER::ULB)
                st[1] |= (1 << i);
            if (corner == CORNER::DLF || corner == CORNER::DRB)
                st[2] |= (1 << i);
            if (corner == CORNER::ULF || corner == CORNER::URB)
                st[3] |= (1 << i);
            if (corner == CORNER::DRF || corner == CORNER::DLB)
                st[4] |= (1 << i);
        }
        /*
        Overall parity
        Corner Parity always matches the edge parity
        Any quarter turn of a face change the corner/edge permutation from even to odd and viceversa
        */
        for (int i = 0; i < 8; i++)
            for (int j = i + 1; j < 8; j++)
                st[5] ^= (cube.corners[i].index < cube.corners[j].index); // trick with XOR operator(swap one bit)
    }
    else if (phase == PHASE4)
    {
        st.assign(2, 0);
        for (int i = 0; i < 12; i++)
        {
            int idx = cube.edges[i].index;
            for (int j = 0; j < 4; j++)
                if (idx & (1 << j))
                    st[0] |= (1ll << (i * 4) + j);
        }

        for (int i = 0; i < 8; i++)
        {
            int idx = cube.corners[i].index;
            for (int j = 0; j < 4; j++)
                if (idx & (1 << j))
                    st[1] |= (1ll << (i * 4) + j);
        }
    }

    // else if(phase == PHASE4){} TODO
    return st;
}

bool Solver::goal(Solver::PHASE phase, RubiksCube &cube)
{
    if (phase == PHASE1)
    {
        // Check if the edges are correctly oriented
        for (int i = 0; i < 12; i++)
        {
            if (cube.edges[i].orientation != 0)
                return false;
        }
        // Goal passed!

        return true;
    }
    else if (phase == PHASE2)
    {
        // Check if the corners are correctly oriented
        for (int i = 0; i < 8; i++)
        {
            if (cube.corners[i].orientation != 0)
                return false;
        }
        // Check if the edges of the E-Slice are on the E-Slice
        EDGE eSlice[4] = {EDGE::LF, EDGE::RF, EDGE::LB, EDGE::RB};

        for (int i = 0; i < 4; i++)
        {
            EDGE ind = eSlice[i];
            if (cube.edges[EDGE::LF].index != ind &&
                cube.edges[EDGE::RF].index != ind &&
                cube.edges[EDGE::LB].index != ind &&
                cube.edges[EDGE::RB].index != ind)
                return false;
        }

        // Goal passed!

        return true;
    }
    else if (phase == PHASE3)
    {
        // Check if the corners are in their correct orbit

        if (CORNER::URF != cube.corners[CORNER::URF].index && CORNER::URF != cube.corners[CORNER::ULB].index)
            return false;
        if (CORNER::DLF != cube.corners[CORNER::DLF].index && CORNER::DLF != cube.corners[CORNER::DRB].index)
            return false;
        if (CORNER::ULF != cube.corners[CORNER::ULF].index && CORNER::ULF != cube.corners[CORNER::URB].index)
            return false;

        // Check if the edges of the M-Slice are on the E-Slice
        EDGE mSlice[4] = {EDGE::UF, EDGE::DF, EDGE::UB, EDGE::DB};

        for (int i = 0; i < 4; i++)
        {
            EDGE ind = mSlice[i];
            if (cube.edges[EDGE::UF].index != ind &&
                cube.edges[EDGE::DF].index != ind &&
                cube.edges[EDGE::UB].index != ind &&
                cube.edges[EDGE::DB].index != ind)
                return false;
        }

        // Goal passed!

        return true;
    }
    else if (phase == PHASE::PHASE4)
    {
        return cube.isSolved();
    }
    return false;
}

// Check if next move can be pruned
bool Solver::prune(MOVE lastMove, MOVE move)
{
    typedef MOVE MOVE;
    /*
     Redundant Moves
     F = F2F2, F2F = F', FF = F2 etc..
    */
    if ((move == MOVE::L || move == MOVE::LPRIME || move == MOVE::L2) &&
        (lastMove == MOVE::L || lastMove == MOVE::LPRIME || lastMove == MOVE::L2))
        return true;
    if ((move == MOVE::R || move == MOVE::RPRIME || move == MOVE::R2) &&
        (lastMove == MOVE::R || lastMove == MOVE::RPRIME || lastMove == MOVE::R2))
        return true;
    if ((move == MOVE::F || move == MOVE::FPRIME || move == MOVE::F2) &&
        (lastMove == MOVE::F || lastMove == MOVE::FPRIME || lastMove == MOVE::F2))
        return true;
    if ((move == MOVE::B || move == MOVE::BPRIME || move == MOVE::B2) &&
        (lastMove == MOVE::B || lastMove == MOVE::BPRIME || lastMove == MOVE::B2))
        return true;
    if ((move == MOVE::U || move == MOVE::UPRIME || move == MOVE::U2) &&
        (lastMove == MOVE::U || lastMove == MOVE::UPRIME || lastMove == MOVE::U2))
        return true;
    if ((move == MOVE::D || move == MOVE::DPRIME || move == MOVE::D2) &&
        (lastMove == MOVE::D || lastMove == MOVE::DPRIME || lastMove == MOVE::D2))
        return true;

    /*
     Commutative Moves
     FB = BF, UD = DU , LR = RL.
     In this case all L moves goes first than R moves (Ex: LR, L2R, etc..)
     U moves goes first than D moves
     F moves goes first than B moves
    */
    if ((lastMove == MOVE::R || lastMove == MOVE::RPRIME || lastMove == MOVE::R2) &&
        (move == MOVE::L || move == MOVE::LPRIME || move == MOVE::L2))
        return true;
    if ((lastMove == MOVE::D || lastMove == MOVE::DPRIME || lastMove == MOVE::D2) &&
        (move == MOVE::U || move == MOVE::UPRIME || move == MOVE::U2))
        return true;
    if ((lastMove == MOVE::B || lastMove == MOVE::BPRIME || lastMove == MOVE::B2) &&
        (move == MOVE::F || move == MOVE::FPRIME || move == MOVE::F2))
        return true;

    return false; // the move cannot be pruned
}

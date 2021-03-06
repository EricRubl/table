//
//  Headerul contine clasele responsabile de varianta romaneasca de joc.
//
//
//
#ifndef rulesro_hpp_guard
#define rulesro_hpp_guard

#include "../game_core.hpp"
#include "../match.hpp"
#include "../phase.hpp"
#include "../dice.hpp"

namespace table
{
    // FORWARD DECLARATIONS
    class RoOpeningRollPhase;
    class RoGamePhase;

    namespace internal
    {
        class RoDiceHelper
        {
            // ajuta la calcularea jucatorului care incepe
            private:
                Color _player;
                Color _winner;
                DicePair _dices;
                bool _is_done;

                void init(Color next_player);

            public:
                RoDiceHelper(Color); // primeste playerul care incepe

                void give_dice(DicePair); // da zarurile arunctae. progreseaza algoritmul
                bool is_done(void) const; // returneaza true daca s-a castigat cineva darea cu zarul
                Color get_winner(void) const; // returneaza castigatorul
                std::pair<bool, DicePair> get_double_pair(void) const; // returneaza o perche de  tipul (bool e dubla?, DicePair daca da atunci dubla)
        };
    }

    class RoMatch : public IMatch
    {
    private:
        int _num_rounds_to_win;
        bool _with_technical;

        std::unique_ptr<RoOpeningRollPhase> _opening_roll_phase;
        std::unique_ptr<RoGamePhase> _game_phase;
    public:
        RoMatch(int=3, bool=true);

        virtual void next_phase(void) override;
        virtual void on_phase_action(void) override;
    };


    class RoGamePhase : public IPhase
    {
    private:
        void _init(void);
        void _init_board_state(void);
        void _compute_legal_moves(void);
    public:
        RoGamePhase(Color, DicePair=DicePair(0, 0)); // creeaza o linie cu un player care incepe si un posibil zar de inceput
        explicit RoGamePhase(const BoardState&, Color, DicePair=DicePair(0, 0)); // construieste si cheama

        virtual void roll_dice(void) override;
        virtual void submit_moves(Turn) override;
    };

    class RoOpeningRollPhase : public IPhase
    {
        // faza de inceput pt jtablele romanesti
        // are valorile de castig "not_won", "double" sau "normal"
    private:
        internal::RoDiceHelper _dice_helper;
        DicePair _starting_dice_pair;
    public:
        RoOpeningRollPhase(Color); // creaza o faza de inceput cu un player care sa arunce primul

        DicePair get_starting_double(void) const; // returneaza zarurile de inceput daca au fost dubla
        virtual void roll_dice(void) override; // da cu zarul
        virtual void preset_roll_dice(DicePair) override;
    protected:
        void _process_dices(void); // pune zarurile in _starting_dice_pair;
    };
}
#endif

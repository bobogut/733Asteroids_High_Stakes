#pragma once

namespace states {
  namespace autonStates {
    enum Autons {
      RedPositiveCorner,
      RedNegativeCorner,
      RedSoloAWP,
      BluePositiveCorner,
      BlueNegativeCorner,
      BlueSoloAWP,
      Skills,
      None,




      RedPositiveAlliance,
      RedPositiveMogo,
      RedPositiveSweepAlliance,
      RedPositiveSweepMogo,



      BluePositiveAlliance,
      BluePositiveMogo,
      BluePositiveSweepAlliance,
      BluePositiveSweepMogo,
    };
  }



  namespace intakeStates {
    enum IntakeStates {
      Resting,
      Mogo,
      Arm,
      Reverse,
      StoreRing,
      FindNextDown,
      FirstStage,
      FirstStageReverse
    };
  }



  namespace armStates {
    enum ArmStates {
      Resting,
      PrimeOne,
      PrimeTwo,
      WallStake,
      AllianceStake,
      TipMogo
    };
  }
}
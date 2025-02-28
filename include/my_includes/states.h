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
      None
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
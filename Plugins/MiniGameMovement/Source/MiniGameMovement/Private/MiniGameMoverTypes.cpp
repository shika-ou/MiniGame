#include "MiniGameMoverTypes.h"


/////////////////////////////
/// FMiniGameTagsSyncState
/// /////////////////////////

FMoverDataStructBase* FMiniGameMovementInputs::Clone() const
{
	FMiniGameMovementInputs* CopyPtr = new FMiniGameMovementInputs(*this);
	return CopyPtr;
}

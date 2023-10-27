package promotion

import (
	"context"
	"encoding/json"
	"fmt"
)

// ParseModelDetail parse model details according to model_type.
func ParseModelDetail(ctx context.Context, typ int32, detail string) (*ModelDetail, error) {
	// Only buy-one-get-one-free model is supported
	if typ != 8 {
		return nil, fmt.Errorf("only buy-one-get-one-free model is supported")
	}
	modelDetail := &ModelDetail{}
	if err := json.Unmarshal([]byte(detail), modelDetail); err != nil {
		return nil, fmt.Errorf("invalid model detail: %s", detail)
	}
	return modelDetail, nil
}

package promotion

type LevelRule struct {
	// Total item judgment switch
	// bool pid_flag = 1;

	// Purchase item rules, key is item ID, value is purchase quantity
	BuyRuleProducts map[string]int64 `json:"buy_rule_products"`

	// Gift item rules, key is item ID, value is gift quantity
	GiftProducts map[string]int64 `json:"gift_products"`

	// Level ID
	LevelId string `json:"level_id"`

	// Whether to enable level limit
	// bool is_level_limit_enabled = 5;

	// Level limit configuration, key is product_id, value is the corresponding limit information
	LevelLimit map[string]ResourceConfInfo `json:"level_limit"`

	// Remaining limit, key is product_id, value is remaining limit
	RemainLimit map[string]LimitRemainDataInfo `json:"remain_limit"`
}

type LimitRemainDataInfo struct {
	// Activity dimension restriction
	// LimitNum activity_frequence = 1;

	// User dimension restriction
	// LimitNum account_frequence = 2;

	// Item dimension restriction
	// LimitNum product_quata = 3;

	// User item dimension restriction
	AccountQuata LimitNum `json:"account_quata"`

	// Resource ID, when resource_id == "qua" it represents the activity level remaining limit
	ResourceId string `json:"resource_id"`
}

type LimitNum struct {
	Day   int64 `json:"day"`   // Daily limit
	Week  int64 `json:"week"`  // Weekly limit
	Month int64 `json:"month"` // Monthly limit
	Total int64 `json:"total"` // Total limit
}

type ResourceConfInfo struct {
	// Limit information
	LimitConfValue LimitConfValue `json:"limit_conf_value"`
}

type LimitConfValue struct {
	// Activity dimension restriction
	// LimitNum activity_feq = 1;

	// User dimension restriction
	// LimitNum acct_feq = 2;

	// Item dimension restriction
	// LimitNum pro_quata = 3;

	// User item dimension restriction
	AcctQuata LimitNum `json:"acct_quata"`
}

type LaderRule struct {
	// Total quantity judgment switch
	// bool number_flag = 1;

	// Left boundary of the cumulative value of multiple items
	BeginNumber int64 `json:"begin_number"`

	// Right boundary of the cumulative value of multiple items
	EndNumber int64 `json:"end_number"`

	// Item ID
	ProductId string `json:"product_id"`

	// Ladder ID
	LaderId string `json:"ladder_id"`

	// Gift items and their quantities
	LadderGiftProducts map[string]int64 `json:"ladder_gift_products"`

	// Gift item proportion
	// float gift_proportion = 7;

	// Whether to enable ladder limit
	// bool is_lader_limit_enabled = 8;

	// Ladder limit configuration
	LaderLimit map[string]ResourceConfInfo `json:"ladder_limit"`

	// Remaining limit
	RemainLimit map[string]LimitRemainDataInfo `json:"remain_limit"`

	// Gift item proportion, gift quantity = total purchase * ladder_gift_products quantity / proportion, round up
	ProductsProportion map[string]int64 `json:"products_proportion"`
}

type GiftRuleInfo struct {
	// Judgment mode of level rule or ladder rule
	JudgeMode int32 `json:"judge_mode"`

	// Activity type
	ActionMode int32 `json:"action_mode"`

	// Activity time
	// int32 action_time = 3;

	// Level rule list, exists when judge_mode == 1
	GiftLevelList []LevelRule `json:"gift_level_list"`

	// Ladder rule list, exists when judge_mode == 2
	GiftLadderList []LaderRule `json:"gift_ladder_list"`

	// Ladder mode, exists when judge_mode == 2
	GiftLadderMode int32 `json:"gift_ladder_mode"`

	GiftLaderPattern int32 `json:"gift_ladder_pattern"`

	LadderProductNum map[string]int64 `json:"ladder_product_num"`
}

type ModelDetail struct {
	// Buy gift rules
	GiftRuleInfo GiftRuleInfo `json:"gift_rule_info"`

	// Accumulation rules
	AccumulateConfig AccumulateConfig `json:"accumulate_config"`

	// Model configuration
	ModelConf ModelConf `json:"model_conf"`

	// Remaining limit information
	// repeated LimitRemainDataInfo limit_remain_list = 4;
}

type AccumulateConfig struct {
	AccumulateValue int64 `json:"accumulate_value"`
}

type ModelConf struct {
	// Model name
	ModelName string `json:"model_name"`

	// Start time
	BeginTime int64 `json:"begin_time"`

	// End time
	EndTime int64 `json:"end_time"`

	// Status
	State int32 `json:"states"`
}

package promotion

import (
	"context"
	"testing"
)

func TestParseModelDetail(t *testing.T) {
	type args struct {
		ctx    context.Context
		typ    int32
		detail string
	}
	tests := []struct {
		name    string
		args    args
		wantErr bool
	}{
		{
			name: "",
			args: args{
				ctx:    context.Background(),
				typ:    8,
				detail: "{\"gift_rule_info\":{\"judge_mode\":1,\"action_mode\":2,\"action_time\":1,\"gift_level_list\":[{\"buy_rule_products\":{\"com.tencent.tdr.sea.midasbuy.pack_premium_01_0_99\":1},\"gift_products\":{\"com.tencent.tdr.sea.midasbuy.pack_premium_03_9_99\":2},\"level_id\":\"448251b7-7b6d-4f\",\"remain_limit\":{\"com.tencent.tdr.sea.midasbuy.pack_premium_03_9_99\":{\"account_quata\":{\"day\":-1,\"week\":-1,\"month\":-1,\"total\":1}}},\"level_limit\":{\"com.tencent.tdr.sea.midasbuy.pack_premium_03_9_99\":{\"limit_conf_value\":{\"acct_quata\":{\"day\":-1,\"week\":-1,\"month\":-1,\"total\":1}}}}}]},\"model_conf\":{\"model_name\":\"首充活动测试\",\"begin_time\":1697126400,\"end_time\":1704038399,\"states\":2}}",
			},
			wantErr: false,
		},
		{
			name: "",
			args: args{
				ctx:    context.Background(),
				typ:    8,
				detail: "{\"gift_rule_info\":{\"judge_mode\":2,\"action_mode\":1,\"action_time\":1,\"gift_ladder_list\":[{\"begin_number\":1,\"end_number\":1000000000,\"ladder_gift_products\":{\"com.tencent.tdr.sea.midasbuy.battlepass_upgrade_9_99\":1},\"ladder_id\":\"e3e0183e-3\",\"ladder_limit\":{\"com.tencent.tdr.sea.midasbuy.battlepass_upgrade_9_99\":{\"limit_conf_value\":{\"acct_quata\":{\"day\":-1,\"week\":-1,\"month\":-1,\"total\":-1}}}},\"remain_limit\":{\"com.tencent.tdr.sea.midasbuy.battlepass_upgrade_9_99\":{\"account_quata\":{\"day\":-1,\"week\":-1,\"month\":-1,\"total\":-1}}}}],\"gift_ladder_mode\":1,\"gift_ladder_pattern\":1,\"ladder_product_num\":{\"com.tencent.tdr.sea.midasbuy.battlepass_upgrade_9_99\":1}},\"model_conf\":{\"model_name\":\"满赠玩法\",\"begin_time\":1697126400,\"end_time\":1704038399,\"states\":2}}",
			},
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := ParseModelDetail(tt.args.ctx, tt.args.typ, tt.args.detail)
			if (err != nil) != tt.wantErr {
				t.Errorf("ParseModelDetail() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			t.Logf("%+v", got)
		})
	}
}

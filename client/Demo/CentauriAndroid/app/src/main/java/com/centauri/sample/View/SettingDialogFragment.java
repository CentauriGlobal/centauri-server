package com.centauri.sample.View;

import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.centauri.sample.Cfg.DC;
import com.tencent.imsdk.samples.R;

public class SettingDialogFragment extends DialogFragment {

    private EditText mEtProductId, mEtSubsId, mEtCountry, mEtCurrencyType;

    public static SettingDialogFragment newInstance() {
        SettingDialogFragment fragment = new SettingDialogFragment();
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_setting, container, false);
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initVew();
        initData();
    }

    private void initVew() {
        mEtProductId = getView().findViewById(R.id.et_productid);
        mEtSubsId = getView().findViewById(R.id.et_subsid);
        mEtCountry = getView().findViewById(R.id.et_country);
        mEtCurrencyType = getView().findViewById(R.id.et_currency_type);
        getView().findViewById(R.id.btn_confirm).setOnClickListener(v -> {
            saveConfig();
            dismiss();
        });
    }

    private void initData() {
        mEtProductId.setText(DC.instance().cfg.mGameProductID);
        mEtSubsId.setText(DC.instance().cfg.mSubsProductID);
        mEtCountry.setText(DC.instance().cfg.mCountry);
        mEtCurrencyType.setText(DC.instance().cfg.mCurrency);
    }

    private void saveConfig() {
        DC.instance().cfg.setmGameProductID(mEtProductId.getText().toString());
        DC.instance().cfg.setmCountry(mEtCountry.getText().toString());
        DC.instance().cfg.setmCurrency(mEtCurrencyType.getText().toString());
        Toast.makeText(getContext(), "success", Toast.LENGTH_LONG).show();
    }
}

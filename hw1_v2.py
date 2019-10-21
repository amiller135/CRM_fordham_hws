#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Oct 17 11:28:59 2019

@author: francescao
"""

import numpy as np
import pandas as pd
from scipy.stats import norm
from scipy.optimize import minimize
from scipy.optimize import fsolve

'''
Take credit issuers you were assigned and perform the following steps:

1.) Find their public ratings from major providers – lookup PD’s for one year, also look up cumulative PD’s for each of 
the ratings over 5-year time periods

2.) Calculate their Altman’s Z scores based on available data – does Z score agree with public ratings? 
What would you change based on this information?

3.) Perform Merton model analysis on each of the issuers, calculate their DD’s and implied PD’s over 5 years

4.) Obtain credit spreads for each of the issuers, calculate and draw default intensity step term structure 
(using Hull approach) – is it different from above steps, why?

5.) Calculate implied JT-model π’s from the generic issuer with the same rating 
(i.e. look at generic 5-yr spread for issuer in the same rating category, solve back for π using 5-year cumulative 
transition matrix), apply those π’s to your issuers PD’s calculated from Hull above, then – what should their 5-year 
credit spreads be? Are they consistent with the market pricing? Why yes, or why no?
'''


def calc_altman_z(total_assets, working_capital=0, retained_earnings=0, ebit=0, mv_equity=0, sales=0, book_value_debt=0):

    '''
    :param total_assets: total assets on balance sheet (cant be 0)
    :param working_capital: current assets-current liabilities
    :param retained_earnings: retained earnings over life of company
    :param ebit: earnings before interest & taxes
    :param mv_equity: market cap (shares outstanding * price)
    :param sales: total sales
    :param book_value_debt: book value of debt
    :return: altman_z_score
    '''

    coefficients = np.array([1.2, 1.4, 3.3, 0.6, 0.999])
    denom = np.repeat(1/total_assets, 4)
    denom = np.append(denom, 1/book_value_debt)
    numer = np.array([working_capital, retained_earnings, ebit, sales, mv_equity])
    return sum(coefficients * numer * denom)


# func to perform merton model analysis.

def input_params(assets, equity, debt, t, equity_vol, rf=0.03):
    '''
    :param assets: total assets on balance sheet
    :param equity: equity value (market cap)
    :param debt: total liabilities on balance sheet
    :param rf: risk free rate (constant)
    :param t: time
    :param equity_vol: volatility of equity
    :return: param estimation for asset_vol and asset value
    :function equation: solves for asset_vol and asset_value
    '''
    def equation(x):
        '''
        :param x: array that holds initial guesses for asset value and asset volatility
        :return: estimation for asset volatility and asset value
        :res1: equity value: E0 = V0 * N(d1)- D*exp(-r*T)*N(d2)
        :res2: linking sigma_equity to sigma_assets sigma_equity * E0 = N(d1) * V0 * asset_vol
        '''


        d1 = (np.log(x[0] / debt) + (rf + np.square(x[1]) / 2) * t)/(x[1] * np.sqrt(t))
        d2 = d1 - x[1] * np.sqrt(t)
        res1 = x[0] * norm.cdf(d1) - np.exp(-rf * t) * debt * norm.cdf(d2) - equity
        res2 = x[0] * norm.cdf(d1) * x[1] - equity * equity_vol
        return (res1,res2)


    results = fsolve(equation, (assets, 0.3))
    return results




def calc_prob_default(assets, equity, debt, t, equity_vol, rf=0.03):
    '''
    :param assets: total assets on balance sheet
    :param equity: equity value (market cap)
    :param debt: total liabilities on balance sheet
    :param rf: risk free rate (constant)
    :param t: time
    :param equity_vol: volatility of equity
    :return: Distance to Default and Probability of Default
    '''
    
    
    asset_vol = list(input_params(assets, equity, debt, rf, t, equity_vol))[1]
    Value_asset = list(input_params(assets, equity, debt, rf, t, equity_vol))[0]
    d1 = (np.log(Value_asset / debt) + (rf + np.square(asset_vol) / 2) * t)/(asset_vol * np.sqrt(t))
    d2 = d1 - asset_vol * np.sqrt(t)
    return d2, norm.cdf(-d2)


if __name__ == "__main__":
    # insert altman z params and func below

    # -------- Merton Model ----------#

    # parameters for merton model (using GE here)
    assets_ge = 309129000000.0  # 309,129,000,000.0
    equity_ge = 76624000000.0  # 76,624,000,000.0
    debt_ge = 232505000000.0  # 232,505,000,000.0
    equity_vol_ge = 0.33715

    dict_profile = {}
    for num in range(1, 6):
        DD, PD = calc_prob_default(assets=assets_ge, equity=equity_ge, debt=debt_ge, t=num,
                                   equity_vol=equity_vol_ge, rf=0.03)
        dict_profile.update({num: (DD, PD)})
    dict_profile

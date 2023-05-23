// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#![allow(clippy::needless_return)]

//! Implements the macros that are exported by the eventheader crate.

extern crate proc_macro;
use proc_macro::{Span, TokenStream};

use crate::enabled_generator::EnabledGenerator;
use crate::enabled_info::EnabledInfo;
use crate::event_generator::EventGenerator;
use crate::event_info::EventInfo;
use crate::provider_generator::ProviderGenerator;
use crate::provider_info::ProviderInfo;

#[proc_macro]
pub fn define_provider(arg_tokens: TokenStream) -> TokenStream {
    let call_site = Span::call_site();
    return match ProviderInfo::try_from_tokens(call_site, arg_tokens) {
        Err(error_tokens) => error_tokens,
        Ok(prov) => ProviderGenerator::new(call_site).generate(prov),
    };
}

#[proc_macro]
pub fn provider_enabled(arg_tokens: TokenStream) -> TokenStream {
    let call_site = Span::call_site();
    return match EnabledInfo::try_from_tokens(call_site, arg_tokens) {
        Err(error_tokens) => error_tokens,
        Ok(enabled) => EnabledGenerator::new(call_site).generate(enabled),
    };
}

#[proc_macro]
pub fn write_event(arg_tokens: TokenStream) -> TokenStream {
    let call_site = Span::call_site();
    return match EventInfo::try_from_tokens(call_site, arg_tokens) {
        Err(error_tokens) => error_tokens,
        Ok(prov) => EventGenerator::new(call_site).generate(prov),
    };
}

mod enabled_generator;
mod enabled_info;
mod enums;
mod errors;
mod event_generator;
mod event_info;
mod expression;
mod field_info;
mod field_option;
mod field_options;
mod ident_builder;
mod parser;
mod provider_generator;
mod provider_info;
mod strings;
mod tree;

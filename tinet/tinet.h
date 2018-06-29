#pragma once

/*
* TINet
* https://tulipindicators.org/
* Copyright (c) 2010-2018 Tulip Charts LLC
* Lewis Van Winkle (LV@tulipcharts.org)
*
* This file is part of TINet (Tulip Indicators).
*
* TINet is free software: you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by the
* Free Software Foundation, either version 3 of the License, or (at your
* option) any later version.
*
* TINet is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
* for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with TINet.  If not, see <http://www.gnu.org/licenses/>.
*
*/


using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

#include <vcclr.h>
#include "../external/indicators.h"

namespace tinet {

	public ref class util
	{
	public:

		static int call_by_name(System::String^ name, cli::array<cli::array<double>^>^ inputs, cli::array<double>^ options, cli::array<cli::array<double>^>^ outputs) {

			const char* n = (const char*)(Marshal::StringToHGlobalAnsi(name)).ToPointer();
			const ti_indicator_info *ind = ti_find_indicator(n);

			if (!ind) {
				return TI_INVALID_OPTION;
			}

			return call_by_index(ind - ti_indicators, inputs, options, outputs);
		}

		static int call_by_name_start(System::String^ name, cli::array<double>^ options) {

			const char* n = (const char*)(Marshal::StringToHGlobalAnsi(name)).ToPointer();
			const ti_indicator_info *ind = ti_find_indicator(n);

			if (!ind) {
				return TI_INVALID_OPTION;
			}

			return call_by_index_start(ind - ti_indicators, options);
		}

		static int call_by_index_start(int index, cli::array<double>^ options) {
			if (index < 0 || index >= TI_INDICATOR_COUNT) {
				return TI_INVALID_OPTION;
			}

			ti_indicator_info *ind = ti_indicators + index;

			double *opts;
			pin_ptr<double> opt_p;
			opt_p = &options[0];
			opts = opt_p;

			return ind->start(opts);
		}

		static int call_by_index(int index, cli::array<cli::array<double>^>^ inputs, cli::array<double>^ options, cli::array<cli::array<double>^>^ outputs) {
			
			//Check for valid index
			if (index < 0 || index >= TI_INDICATOR_COUNT) {
				return TI_INVALID_OPTION;
			}

			ti_indicator_info *ind = ti_indicators + index;


			//Check that the correct number of options were passed in.
			if (ind->inputs != inputs->GetLength(0)) {
				return TI_INVALID_OPTION;
			}
			if (ind->options != options->GetLength(0)) {
				return TI_INVALID_OPTION;
			}
			if (ind->outputs != outputs->GetLength(0)) {
				return TI_INVALID_OPTION;
			}

			//Check that all input arrays are the same length.
			int i;
			for (i = 1; i < ind->inputs; ++i) {
				if (inputs[i]->GetLength(0) != inputs[0]->GetLength(0)) {
					return TI_INVALID_OPTION;
				}
			}

			int needed_output = inputs[0]->GetLength(0) - call_by_index_start(index, options);
			for (i = 0; i < ind->outputs; ++i) {
				if (outputs[i]->GetLength(0) < needed_output) {
					return TI_INVALID_OPTION;
				}
			}


			double *ins[TI_MAXINDPARAMS];
			double *opts;
			double *outs[TI_MAXINDPARAMS];
			
			//This is a mess, but pin_ptr is only allowed on the stack, not in containers or arrays.
			pin_ptr<double> in_p0, in_p1, in_p2, in_p3, in_p4, in_p5, in_p6, in_p7, in_p8, in_p9;
			pin_ptr<double> opt_p;
			pin_ptr<double> out_p0, out_p1, out_p2, out_p3, out_p4, out_p5, out_p6, out_p7, out_p8, out_p9;

#if TI_MAXINDPARAMS != 10
#error Expect max of 10 params
#endif

			if (ind->inputs >= 1)  { in_p0 = &inputs[0][0]; ins[0] = in_p0; }
			if (ind->inputs >= 2)  { in_p1 = &inputs[1][0]; ins[1] = in_p1; }
			if (ind->inputs >= 3)  { in_p2 = &inputs[2][0]; ins[2] = in_p2; }
			if (ind->inputs >= 4)  { in_p3 = &inputs[3][0]; ins[3] = in_p3; }
			if (ind->inputs >= 5)  { in_p4 = &inputs[4][0]; ins[4] = in_p4; }
			if (ind->inputs >= 6)  { in_p5 = &inputs[5][0]; ins[5] = in_p5; }
			if (ind->inputs >= 7)  { in_p6 = &inputs[6][0]; ins[6] = in_p6; }
			if (ind->inputs >= 8)  { in_p7 = &inputs[7][0]; ins[7] = in_p7; }
			if (ind->inputs >= 9)  { in_p8 = &inputs[8][0]; ins[8] = in_p8; }
			if (ind->inputs >= 10) { in_p9 = &inputs[9][0]; ins[9] = in_p9; }
			
			opt_p = &options[0];
			opts = opt_p;

			if (ind->outputs >= 1)  { out_p0 = &outputs[0][0]; outs[0] = out_p0; }
			if (ind->outputs >= 2)  { out_p1 = &outputs[1][0]; outs[1] = out_p1; }
			if (ind->outputs >= 3)  { out_p2 = &outputs[2][0]; outs[2] = out_p2; }
			if (ind->outputs >= 4)  { out_p3 = &outputs[3][0]; outs[3] = out_p3; }
			if (ind->outputs >= 5)  { out_p4 = &outputs[4][0]; outs[4] = out_p4; }
			if (ind->outputs >= 6)  { out_p5 = &outputs[5][0]; outs[5] = out_p5; }
			if (ind->outputs >= 7)  { out_p6 = &outputs[6][0]; outs[6] = out_p6; }
			if (ind->outputs >= 8)  { out_p7 = &outputs[7][0]; outs[7] = out_p7; }
			if (ind->outputs >= 9)  { out_p8 = &outputs[8][0]; outs[8] = out_p8; }
			if (ind->outputs >= 10) { out_p9 = &outputs[9][0]; outs[9] = out_p9; }

			return ind->indicator(inputs[0]->GetLength(0), ins, opts, outs);
		}

	}; //class util


	public ref class indicator
	{
	private:
		int index;
		const ti_indicator_info *ind;
		cli::array<String^>^ inputs;
		cli::array<String^>^ options;
		cli::array<String^>^ outputs;

	public:
		indicator(int index) {
			this->index = index;
			this->ind = ti_indicators + index;

			int i;

			this->inputs = gcnew cli::array<String^>(this->ind->inputs);
			for (i = 0; i < this->ind->inputs; ++i) {
				this->inputs[i] = gcnew String(this->ind->input_names[i]);
			}

			this->options = gcnew cli::array<String^>(this->ind->options);
			for (i = 0; i < this->ind->options; ++i) {
				this->options[i] = gcnew String(this->ind->option_names[i]);
			}

			this->outputs = gcnew cli::array<String^>(this->ind->outputs);
			for (i = 0; i < this->ind->outputs; ++i) {
				this->outputs[i] = gcnew String(this->ind->output_names[i]);
			}
		}

		const char* name() {
			return ind->name;
		}

		const char* full_name() {
			return ind->name;
		}

		int run(cli::array<cli::array<double>^>^ inputs, cli::array<double>^ options, cli::array<cli::array<double>^>^ outputs) {
			return util::call_by_index(this->index, inputs, options, outputs);
		}

		int start(cli::array<double>^ options) {
			return util::call_by_index_start(this->index, options);
		}

		cli::array<String^>^ input_names() {
			return this->inputs;
		}

		cli::array<String^>^ option_names() {
			return this->options;
		}

		cli::array<String^>^ output_names() {
			return this->outputs;
		}

	}; //class indicator


	

	public ref class indicators
	{
	public:
#include "inds.cpp"
	}; //namespace indicators

}//namespace tinet



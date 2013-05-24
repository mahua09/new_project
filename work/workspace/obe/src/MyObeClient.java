import java.util.List;

import com.travelsky.sbeclient.SBEResponse;
import com.travelsky.sbeclient.authorization.AccountActiveRequest;
import com.travelsky.sbeclient.authorization.AuthorizationRequest;
import com.travelsky.sbeclient.authorization.AuthorizationResponse;
import com.travelsky.sbeclient.exceptions.SBEClientException;
import com.travelsky.sbeclient.obe.AVClient;
import com.travelsky.sbeclient.obe.ObeClient;
import com.travelsky.sbeclient.obe.exceptions.ObeException;
import com.travelsky.sbeclient.obe.request.AVRequest;
import com.travelsky.sbeclient.obe.response.AVResponse;
import com.travelsky.sbeclient.obe.response.AvItem;
import com.travelsky.sbeclient.obe.response.AvSegment;

public class MyObeClient {
	public static ObeClient obeClient = new ObeClient();
	private final int RESULT_CODE_ERROR_TOKEN_INVALID = 20005;
	private final int RESULT_CODE_ERROR_SN_INVALID = 2009;

	public ClientInfo client;

	public MyObeClient(ClientInfo client) {
		this.client = client;
	}

	public void activeAccount() {
		AccountActiveRequest acountRequest = new AccountActiveRequest(client
				.getSbeId(), client.getPwd(), client.getIsActive());
		try {
			SBEResponse response = obeClient.activeAccount(acountRequest);
			if (response.getResultCode() == 0) {
				System.out.println("����ɹ�");
			} else {
				System.out.println(response.getErrorDescription());
				System.out.println("����ʧ��");
			}
		} catch (SBEClientException e) {
			e.printStackTrace();
		}

	}

	public void inactiveAccount() {
		AccountActiveRequest request = new AccountActiveRequest(client
				.getSbeId(), client.getPwd(), client.getIsActive());
		try {
			SBEResponse response = obeClient.inactiveAccount(request);
			if (response.getResultCode() == 0) {
				System.out.println("����ɹ�");
			} else {
				System.out.println(response.getErrorDescription());
				System.out.println("����ʧ��");
			}
		} catch (SBEClientException e) {
			e.printStackTrace();
		}
	}

	public void applyToken() {
		AuthorizationRequest authorizationRequest = new AuthorizationRequest(
				client.getSbeId(), client.getPwd());
		try {
			AuthorizationResponse authorizationResponse = obeClient
					.applyToken(authorizationRequest);
			int resultCode = authorizationResponse.getResultCode();
			if (resultCode == RESULT_CODE_ERROR_TOKEN_INVALID) {
				System.out.println("������Ч,��������������");
			} else if (resultCode == RESULT_CODE_ERROR_SN_INVALID) {
				System.out.println("��Ч�����к�,����𣬼����������������");
			} else {
				System.out.println(authorizationResponse.getErrorDescription());
			}
			String token = authorizationResponse.getToken();
			if (token != null) {
				System.out.println("�ɹ���������:" + token);
			}
		} catch (SBEClientException e) {
			e.printStackTrace();
		}
	}

	public AVResponse av(AVRequest request) {
		AVClient avClient = new AVClient();
		try {
			AVResponse avresponse = avClient.av(request);
			List<AvItem> avItems=avresponse.getAvItems();
			
			for (AvItem avItem : avItems) {
				System.out.println(avItem.getS_number());
				for (AvSegment avegment : avItem.getSegments()) {
					
					String[] bunkdata=avegment.getCangwei_data();
					char[] bunkindex=avegment.getCangwei_index();
					for (int i = 0; i < bunkindex.length; i++) {
						System.out.println(bunkindex[i]+":"+bunkdata[i]);
					}
					char[] cangwei_data_sort=avegment.getCangwei_index_sort();
					for (char c : cangwei_data_sort) {
						System.out.println(c);
					}
					
				}
			}
			
			return avresponse;
		} catch (ObeException e) {
			e.printStackTrace();
			return null;
		}
	}
}

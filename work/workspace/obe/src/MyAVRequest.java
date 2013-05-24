import com.travelsky.sbeclient.obe.request.AVRequest;

public class MyAVRequest extends AVRequest {
	public String org;// �������� ����
	public String dst;// �ִ���� ����
	public String date;// ��ѯ���ڣ���ʽyyyy-MM-dd ����
	public String airline;// ���չ�˾ ����
	// public String direct;//�Ƿ�ֱ�� false
	public boolean direct = false;// �Ƿ�ֱ�� ��ѡ
	// public String nonstop;//�Ƿ��о�ͣ�� ��ѡ
	public boolean nonstop = false;// �Ƿ��о�ͣ�� ��ѡ
	public String stopCity;// ת��أ���ת���У� ��ѡ
	public String fltNo;// ����ų��˲�ѯ�����⣬��������������������ܹ��� ��ѡ
	public String transit;// ��ת���� ��ѡ
	public int page = 30;// �������ҳ���� ��ѡ

	public MyAVRequest() {
	}

	public MyAVRequest(String org, String dst, String date, String airline,
			boolean direct, boolean nonstop, String stopCity, String fltNo,
			String transit, int page) {
		this.org = org;
		this.dst = dst;
		this.date = date;
		this.airline = airline;
		this.direct = direct;
		this.nonstop = nonstop;
		this.stopCity = stopCity;
		this.fltNo = fltNo;
		this.transit = transit;
		this.page = page;
	}

	public String getOrg() {
		return org;
	}

	public void setOrg(String org) {
		this.org = org;
	}

	public String getDst() {
		return dst;
	}

	public void setDst(String dst) {
		this.dst = dst;
	}

	public String getDate() {
		return date;
	}

	public void setDate(String date) {
		this.date = date;
	}

	public String getAirline() {
		return airline;
	}

	public void setAirline(String airline) {
		this.airline = airline;
	}

	public boolean isDirect() {
		return direct;
	}

	public void setDirect(boolean direct) {
		this.direct = direct;
	}

	public boolean isNonstop() {
		return nonstop;
	}

	public void setNonstop(boolean nonstop) {
		this.nonstop = nonstop;
	}

	public String getFltNo() {
		return fltNo;
	}

	public void setFltNo(String fltNo) {
		this.fltNo = fltNo;
	}

	public String getTransit() {
		return transit;
	}

	public void setTransit(String transit) {
		this.transit = transit;
	}

	public String getStopCity() {
		return stopCity;
	}

	public void setStopCity(String stopCity) {
		this.stopCity = stopCity;
	}

	public int getPage() {
		return page;
	}

	public void setPage(int page) {
		this.page = page;
	}

	
}
